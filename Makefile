TARGET_DIRS =\
	Utils \
	Math \
	LineAnalysis \
	Iriss \
	raspi/FlightController \
	raspi/FakeArdupilot \
	raspi/SerialAAAAA \
	raspi/OrderTaker

HOST_DIRS =\
	Utils \
	Math \
	LineAnalysis \
	Iriss \
	desktop/SendOrders


default: target

libs= libUtils.a libMath.so libIriss.so libLineAnalysis.so
dirs= Utils Math LineAnalysis Iriss

define build_target
$(1)-$(2):
	@echo Building $(1) for $(2)
	make -s -C $(1) $(2)
endef

define rule_build
$(1)-$(2)
endef

define clean_target
$(1)-$(2)-clean:
	@echo Cleaning $(1) for $(2)
	make -s -C $(1) clean
endef

define rule_clean
$(1)-$(2)-clean
endef

define very_clean_target
$(1)-$(2)-very-clean:
	@echo Very cleaning $(1) for $(2)
	make -s -C $(1) very_clean
endef

define rule_very_clean
$(1)-$(2)-very-clean
endef

all: super-clean target clean-host host

target: $(foreach proj,$(TARGET_DIRS),$(call rule_build,$(proj),target))
	@echo Done Building for RaspberryPi

host: $(foreach proj,$(HOST_DIRS),$(call rule_build,$(proj),host))
	@echo Done Building for Desktop

clean: clean-target clean-host
	@echo Done Cleaning target and host

clean-target: $(foreach proj,$(TARGET_DIRS),$(call rule_clean,$(proj),target))
	@echo Done Cleaning target

clean-host: $(foreach proj,$(HOST_DIRS),$(call rule_clean,$(proj),host))
	@echo Done Cleaning  host

very_clean: very-clean-target very-clean-host
	@echo Done Very Cleaning target and host

very-clean-target: $(foreach proj,$(TARGET_DIRS),$(call rule_very_clean,$(proj),target))
	@echo Done Very Cleaning target

very-clean-host: $(foreach proj,$(HOST_DIRS),$(call rule_very_clean,$(proj),host))
	@echo Done Very Cleaning host

super-clean: very_clean super-clean-host super-clean-target
	@echo Super Clean!

super-clean-host:
	@echo ${libs} | sed 's/ /\n/g' | sed 's/lib\(.*\..*\)/host\/debug\/lib\/lib\1/g' | xargs rm -f
	@echo ${dirs} | sed 's/ /\n/g' | sed 's/\(.*\)/host\/debug\/include\/\1/g' | xargs rm -rf
	@echo ${libs} | sed 's/ /\n/g' | sed 's/lib\(.*\..*\)/host\/release\/lib\/lib\1/g' | xargs rm -f
	@echo ${dirs} | sed 's/ /\n/g' | sed 's/\(.*\)/host\/release\/include\/\1/g' | xargs rm -rf

super-clean-target:
	@echo ${libs} | sed 's/ /\n/g' | sed 's/lib\(.*\..*\)/target\/debug\/lib\/lib\1/g' | xargs rm -f
	@echo ${dirs} | sed 's/ /\n/g' | sed 's/\(.*\)/target\/debug\/include\/\1/g' | xargs rm -rf
	@echo ${libs} | sed 's/ /\n/g' | sed 's/lib\(.*\..*\)/target\/release\/lib\/lib\1/g' | xargs rm -f
	@echo ${dirs} | sed 's/ /\n/g' | sed 's/\(.*\)/target\/release\/include\/\1/g' | xargs rm -rf


# Define the rules for building
$(foreach proj,$(TARGET_DIRS),$(eval $(call build_target,$(proj),target)))
$(foreach proj,$(HOST_DIRS),$(eval $(call build_target,$(proj),host)))

# Define the rules for cleaning
$(foreach proj,$(TARGET_DIRS),$(eval $(call clean_target,$(proj),target)))
$(foreach proj,$(HOST_DIRS),$(eval $(call clean_target,$(proj),host)))

# Define the rules for cleaning everything
$(foreach proj,$(TARGET_DIRS),$(eval $(call very_clean_target,$(proj),target)))
$(foreach proj,$(HOST_DIRS),$(eval $(call very_clean_target,$(proj),host)))
