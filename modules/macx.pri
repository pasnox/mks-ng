# Mac universal build from 10.3 & up

macx:universal {
    SDK_PATH = $$(MAC_SDKS_PATH)
    isEmpty( SDK_PATH ):SDK_PATH = /Developer/SDKs
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
    QMAKE_MAC_SDK = $${SDK_PATH}/MacOSX10.6.sdk
    CONFIG *= app_bundle
    CONFIG *= x86
    CONFIG *= ppc
    #CONFIG *= x86_64
    # this link is required for building the ppc port to avoid the undefined __Unwind_Resume symbol
    CONFIG( ppc ):LIBS *= -lgcc_eh
}