/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-



#ifdef USERHOOK_INIT
void userhook_init()
{
    // do nothing for now    
}
#endif

#ifdef USERHOOK_FASTLOOP
void userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void userhook_50Hz()
{
    // insert my awesome (c++) code here!

    // If not online, send an ACK and check for a response
    int16_t response
    if(!rpiOnline) {
        hal.uartA.write(cmdBuf, Iriss::Command::PACKED_SIZE);
        // wait a little bit for a response
        response = hal.uartA.read();
        if(response > -1) {
            uint8_t asUChar = static_cast<uint8_t>(response&0xFFu);
            if(response == Iriss::BEGIN_UART_MESSAGE) {
                for(int i = 0; i < Iriss::Command::PACKED_SIZE) {
                    cmdBuf[i] = static_cast<uint8_t>(hal.uartA.read()&0xFFu);
                }
                cmd.unpack(cmdBuf, Iriss::Command::PACKED_SIZE);
                if(cmd.get() == Iriss::Command::ACK) {
                    rpiOnline = true;
                }
            }
        }
    }
    else {
        // check for a command and respond accordingly
        response = hal.uartA.read();
        if(response > -1) {
            uint8_t asUChar = static_cast<uint8_t>(response&0xFFu);
            if(response == Iriss::BEGIN_UART_MESSAGE) {
                for(int i = 0; i < Iriss::Command::PACKED_SIZE; ++i) {
                    cmdBuf[i] = static_cast<uint8_t>(hal.uartA.read()&0xFF);
                }
                cmd.unpack(cmdBuf, Iriss::Command::PACKED_SIZE);
                if(cmd.get() == Iriss::Command::ACK) {
                    rpiOnline = true;
                }
            }
        }

    }
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif
