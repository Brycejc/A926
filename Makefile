### DO NOT EDIT THIS FILE ###
ifeq ($(strip x$(SDK_BUILD)),x)
include build/core/main.mk
else
include build/core/sdk_main.mk
endif
### DO NOT EDIT THIS FILE ###
