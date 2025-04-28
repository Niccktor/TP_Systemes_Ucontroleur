# TP_Systemes_Ucontroleur
SEHTEL Azzedine

BEGUIN Thomas

# Démarrage
## Testez la LED LD2
    Activer les GPIO dans l'IOC
        PA5 correspond à notre LED2, rajotuer un Label pour faciliter le code
## Usart 2 + printf
    Activer l'USART2 en Asynchronous Pin PA2(TX) et PA3(RX)

# GPIO Expander
## Configuration
    Ref GPIO Expander : MCP23S17
    Sur la Nucelo, le SPI3 et utilisé pour le MCP23S17.
	Configuration du SPI dans l'IOC
	SPI3:
		Full Duplex Master
		Data Size = 8
		Prescaler = 16
		PB5 = SPI3_MOSI
		PC10 = SPI3_SCK
		PC11 = SPI3_MISO
#LeCODECAudio SGTL5000
##Configuration de l'I2C_2

    I2C_SCL = PB10
    I2C_SDA = PB11
##Configuration SGTL5000
	
    CHIP_ANA_POWER = 0x4260 -> 
    CHIP_LINREG_CTRL = 0x006C ->
    CHIP_REF_CTRL = 0x004E -> VAG_VAL = 1.575V, BIAS_CTRL = -50%, SMALL_POP = 1
    CHIP_LINE_OUT_CTRL = 0x0322 -> LO_VAGCNTRL = 1.65V, OUT_CURRENT = 0.36mA 
    CHIP_SHORT_CTRL = 0x1106 -> MODE_CM = 2, MODE_LR = 1, LVLADJC = 200mA, LVLADJL = 75mA, LVLADJR = 50mA
    CHIP_ANA_CTRL = 0x0133 -> Unmute all + SELECT_ADC = LINEIN
    CHIP_ANA_POWER = 0x6AFF -> VAG_POWERUP, VCOAMP_POWERUP = 0, LINREG_D_POWERUP, PLL_POWERUP = 0, VDDC_CHRGPMP_POWERUP, STARTUP_POWERUP = 0, LINREG_SIMPLE_POWERUP, DAC_MONO = stereo
    CHIP_DIG_POWER = 0x0073 -> I2S_IN_POWERUP, I2S_OUT_POWERUP, DAP_POWERUP, DAC_POWERUP, ADC_POWERUP
    CHIP_LINE_OUT_VOL = 0x0505 ->
    CHIP_CLK_CTRL = 0x0004 -> SYS_FS = 48kHz
    CHIP_I2S_CTRL = 0x0130 -> DLEN = 16 bits
    CHIP_ADCDAC_CTRL = 0x0000 -> Unmute
    CHIP_DAC_VOL = 0x3C3C ->
    HIP_MIC_CTRL = 0x0251 -> BIAS_RESISTOR = 2, BIAS_VOLT = 5, GAIN = 1

    
