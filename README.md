# TP_Systemes_Ucontroleur

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
###Configuration du SPI dans l'IOC
	SPI3:
		Full Duplex Master
		Data Size = 8
		Prescaler = 16
		PB5 = SPI3_MOSI
		PC10 = SPI3_SCK
		PC11 = SPI3_MISO
