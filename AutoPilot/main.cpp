#include <AP_Common.h>
#include <AP_Math.h>
#include <AP_Param.h>
#include <AP_Progmem.h>
#include <AP_ADC.h>
#include <AP_InertialSensor.h>

#include <AP_HAL.h>
#include <AP_HAL_AVR.h>

#include <PID.h>

// My Stuff!
#include "Packable.hpp"
#include "Command.hpp"
#include "Orientation.hpp"

// ArduPilot Hardware Abstraction Layer
const AP_HAL::HAL& hal = AP_HAL_AVR_APM2;

// MPU6050 accel/gyro chip
AP_InertialSensor_MPU6000 ins;

// Radio min/max values for each stick for my radio (worked out at beginning of article)
#define RC_THR_MIN   1070
#define RC_YAW_MIN   1068
#define RC_YAW_MAX   1915
#define RC_PIT_MIN   1077
#define RC_PIT_MAX   1915
#define RC_ROL_MIN   1090
#define RC_ROL_MAX   1913

// Motor numbers definitions
#define MOTOR_FL   2    // Front left    
#define MOTOR_FR   0    // Front right
#define MOTOR_BL   1    // back left
#define MOTOR_BR   3    // back right

// Arduino map function
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define wrap_180(x) (x < -180 ? x+360 : (x > 180 ? x - 360: x))

// PID array (6 pids, two for each axis)
PID pids[6];
#define PID_PITCH_RATE 0
#define PID_ROLL_RATE 1
#define PID_PITCH_STAB 2
#define PID_ROLL_STAB 3
#define PID_YAW_RATE 4
#define PID_YAW_STAB 5

static bool waitOnAck = false;

static uint32_t lastSent;

void setup() 
{
    // Enable the motors and set at 490Hz update
//    hal.rcout->set_freq(0xF, 490);
//    hal.rcout->enable_mask(0xFF);

    // PID Configuration
    pids[PID_PITCH_RATE].kP(0.7);
    pids[PID_PITCH_RATE].kI(1);
    pids[PID_PITCH_RATE].imax(50);

    pids[PID_ROLL_RATE].kP(0.7);
    pids[PID_ROLL_RATE].kI(1);
    pids[PID_ROLL_RATE].imax(50);

    pids[PID_YAW_RATE].kP(2.7);
    pids[PID_YAW_RATE].kI(1);
    pids[PID_YAW_RATE].imax(50);

    pids[PID_PITCH_STAB].kP(4.5);
    pids[PID_ROLL_STAB].kP(4.5);
    pids[PID_YAW_STAB].kP(10);

    // Turn off Barometer to avoid bus collisions
    hal.gpio->pinMode(40, GPIO_OUTPUT);
    hal.gpio->write(40, 1);

    // Turn on MPU6050 - quad must be kept still as gyros will calibrate
    ins.init(AP_InertialSensor::COLD_START, 
             AP_InertialSensor::RATE_100HZ,
                        NULL);

    // initialise sensor fusion on MPU6050 chip (aka DigitalMotionProcessing/DMP)
    hal.scheduler->suspend_timer_procs();  // stop bus collisions
    ins.dmp_init();
    hal.scheduler->resume_timer_procs();


    hal.uartA->begin(57600);   // for radios
    // We're ready to go! Now over to loop()
}

Iriss::Command check_for_command(void)
{
    // state if we have not received a full message yet
    static bool beganMessage = false;
    static bool haveSize = false;
    static uint32_t messageSize = 0;
    
    Iriss::Command cmd(Iriss::Command::TX_ERR);

    int bytesAvail = hal.console->available();

    // if we have not begun reading in a message, check for one
    if(!beganMessage && bytesAvail > 0) {
        uint8_t byte = hal.console->read();
        if(byte == Iriss::BEGIN_UART_MESSAGE) {
            beganMessage = true;
        }
    }

    if(beganMessage && !haveSize && bytesAvail >= 4) {
        uint8_t lenBuf[4];
        for(int i = 0; i < 4; ++i) {
            lenBuf[i] = hal.console->read(); 
        }
        Utils::Packable::unpack_uint32(lenBuf, messageSize);
        if(messageSize == Iriss::Command::PACKED_SIZE) {
            haveSize = true;
        }
        else {
            haveSize = false;
            beganMessage = false;
        }
    }

    if(beganMessage && haveSize && bytesAvail >= messageSize) {
        uint8_t cmdBuf[Iriss::Command::PACKED_SIZE];
        for(unsigned int i = 0; i < messageSize; ++i) {
            cmdBuf[i] = hal.console->read();
        }
        cmd.unpack(cmdBuf, Iriss::Command::PACKED_SIZE);
        beganMessage = false;
        haveSize = false;
    }
    return cmd;
}

void do_respond(const Iriss::Command& cmd, int16_t *channels)
{
    uint32_t directives = cmd.get();
    if(directives == Iriss::Command::TX_ERR) {
        return;
    }
    
    if(directives & Iriss::Command::ACK) {
        // reply with an ACK if not waiting on one
        if(!waitOnAck) {
            Iriss::Command resp(Iriss::Command::ACK);
            hal.console->write(&Iriss::BEGIN_UART_MESSAGE, 1);
            
            uint8_t lenBuf[4];
            Utils::Packable::pack_uint32(Iriss::Command::PACKED_SIZE, lenBuf);
            hal.console->write(lenBuf, sizeof(Iriss::Command::PACKED_SIZE));

            uint8_t cmdBuf[Iriss::Command::PACKED_SIZE];
            resp.pack(cmdBuf, Iriss::Command::PACKED_SIZE);
            hal.console->write(cmdBuf, Iriss::Command::PACKED_SIZE);

        }
        else {
            waitOnAck = false;
        }
    }

    if(directives & Iriss::Command::SEND_AGAIN) {
        // send the last command    
        Iriss::Command resp;
        resp.set(lastSent);
        hal.console->write(&Iriss::BEGIN_UART_MESSAGE, 1);
        
        uint8_t lenBuf[4];
        Utils::Packable::pack_uint32(Iriss::Command::PACKED_SIZE, lenBuf);
        hal.console->write(lenBuf, sizeof(Iriss::Command::PACKED_SIZE));

        uint8_t cmdBuf[Iriss::Command::PACKED_SIZE];
        resp.pack(cmdBuf, Iriss::Command::PACKED_SIZE);
        hal.console->write(cmdBuf, Iriss::Command::PACKED_SIZE);

    }
  
    if(directives & Iriss::Command::GET_ORIENTATION) {
        // get the orientation from ins and send it
        ins.update();
        float roll,pitch,yaw;  
        ins.quaternion.to_euler(&roll, &pitch, &yaw);
        roll = ToDeg(roll);
        pitch = ToDeg(pitch);
        yaw = ToDeg(yaw);
        
        Iriss::Orientation orientation(roll, pitch, yaw);
        hal.console->write(&Iriss::BEGIN_UART_MESSAGE, 1);
        
        uint8_t lenBuf[4];
        Utils::Packable::pack_uint32(Iriss::Orientation::PACKED_SIZE, lenBuf);
        hal.console->write(lenBuf, sizeof(Iriss::Command::PACKED_SIZE));

        uint8_t orientBuf[Iriss::Orientation::PACKED_SIZE];
        orientation.pack(orientBuf, Iriss::Orientation::PACKED_SIZE);
        hal.console->write(orientBuf, Iriss::Orientation::PACKED_SIZE);
    }
   
    if(directives & Iriss::Command::NUDGE_ROLL_LEFT) {
        // update channels 0
        --channels[0];
    }
   
    if(directives & Iriss::Command::NUDGE_ROLL_RIGHT) {
        // update channels 0 in the other way from LEFT 
        ++channels[0];
    }
   
    if(directives & Iriss::Command::NUDGE_UP) {
        // update channels 2 
        ++channels[2];
    }
   
    if(directives & Iriss::Command::NUDGE_DOWN) {
        // update channels 2 in the other way from UP 
        --channels[2];
    }
   
    if(directives & Iriss::Command::NUDGE_YAW_CCW) {
        // update channels 3 
        ++channels[3];
    }
   
    if(directives & Iriss::Command::NUDGE_YAW_CW) {
        // update channels 3 in the other way from CCW 
        --channels[3];
    }
   
    if(directives & Iriss::Command::NUDGE_PITCH_DOWN) {
        // update channels 1 
        ++channels[1];
    }
   
    if(directives & Iriss::Command::NUDGE_PITCH_UP) {
        // update channels 1 in the other way from  DOWN
        --channels[1];
    }
}

void loop() 
{
    static float yaw_target = 0;  
    // Wait until new orientation data (normally 5ms max)
    while (ins.num_samples_available() == 0);

    static uint32_t lastPkt = 0;

    static int16_t channels[8] = {0,0,0,0,0,0,0,0};
     
    Iriss::Command doSomething = check_for_command();
    do_respond(doSomething, channels);
     
    // turn throttle off if no update for 0.5seconds
    if(hal.scheduler->millis() - lastPkt > 500) {
        channels[2] = 0;
    }

    long rcthr, rcyaw, rcpit, rcroll;  // Variables to store radio in 

    // Read RC transmitter 
    rcthr = channels[2];
    rcyaw = channels[3];
    rcpit = channels[1];
    rcroll = channels[0];

    // Ask MPU6050 for orientation
    ins.update();
    float roll,pitch,yaw;  
    ins.quaternion.to_euler(&roll, &pitch, &yaw);
    roll = ToDeg(roll) ;
    pitch = ToDeg(pitch) ;
    yaw = ToDeg(yaw) ;

    // Ask MPU6050 for gyro data
    Vector3f gyro = ins.get_gyro();
    float gyroPitch = ToDeg(gyro.y), gyroRoll = ToDeg(gyro.x), gyroYaw = ToDeg(gyro.z);

    // Do the magic
    if(rcthr > RC_THR_MIN + 100) {  // Throttle raised, turn on stablisation.
        // Stablise PIDS
        float pitch_stab_output = constrain(pids[PID_PITCH_STAB].get_pid((float)rcpit - pitch, 1), -250, 250); 
        float roll_stab_output = constrain(pids[PID_ROLL_STAB].get_pid((float)rcroll - roll, 1), -250, 250);
        float yaw_stab_output = constrain(pids[PID_YAW_STAB].get_pid(wrap_180(yaw_target - yaw), 1), -360, 360);

        // is pilot asking for yaw change - if so feed directly to rate pid (overwriting yaw stab output)
        if(abs(rcyaw ) > 5) {
            yaw_stab_output = rcyaw;
            yaw_target = yaw;   // remember this yaw for when pilot stops
        }

        // rate PIDS
        long pitch_output =  (long) constrain(pids[PID_PITCH_RATE].get_pid(pitch_stab_output - gyroPitch, 1), - 500, 500);  
        long roll_output =  (long) constrain(pids[PID_ROLL_RATE].get_pid(roll_stab_output - gyroRoll, 1), -500, 500);  
        long yaw_output =  (long) constrain(pids[PID_ROLL_RATE].get_pid(yaw_stab_output - gyroYaw, 1), -500, 500);  

        // mix pid outputs and send to the motors.
//        hal.rcout->write(MOTOR_FL, rcthr + roll_output + pitch_output - yaw_output);
//        hal.rcout->write(MOTOR_BL, rcthr + roll_output - pitch_output + yaw_output);
//        hal.rcout->write(MOTOR_FR, rcthr - roll_output + pitch_output + yaw_output);
//        hal.rcout->write(MOTOR_BR, rcthr - roll_output - pitch_output - yaw_output);
    }
    else {
        // motors off
//        hal.rcout->write(MOTOR_FL, 1000);
//        hal.rcout->write(MOTOR_BL, 1000);
//        hal.rcout->write(MOTOR_FR, 1000);
//        hal.rcout->write(MOTOR_BR, 1000);
           
        // reset yaw target so we maintain this on takeoff
        yaw_target = yaw;

        // reset PID integrals whilst on the ground
        for(int i=0; i<6; i++) {
            pids[i].reset_I();
        }

    }
}

AP_HAL_MAIN();

