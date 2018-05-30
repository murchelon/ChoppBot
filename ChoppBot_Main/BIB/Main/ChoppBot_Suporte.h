
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FUNCOES DE SUPORTE
//  ==================
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void(*resetFunc) (void) = 0;  // funcao interna do arduino que reseta o arduino

// Loga uma informacao no terminal
void LogTerm (String Texto)
{
	Serial.println(Texto);
	Serial.flush();
	serialEventRun();
}


// Loga uma informacao no terminal - OVERRIDE int
void LogTerm (int Texto)
{
	Serial.println(Texto);
	Serial.flush();
	serialEventRun();
}


// Loga uma informacao no terminal - OVERRIDE int
void LogTerm (unsigned long Texto)
{
	Serial.println(Texto);
	Serial.flush();
	serialEventRun();
}

// Loga uma informacao no terminal - OVERRIDE int
void LogTerm (double Texto)
{

	Serial.println(Texto);
	Serial.flush();
	serialEventRun();
}


String Right(String Alvo, int Qtde)
{
	return Alvo.substring(Alvo.length() - Qtde);
}


String Left(String Alvo, int Qtde)
{
	return Alvo.substring(0, Qtde);
}


String ReadConsoleInput()
{

	if (Serial.available() > 0) 
	{
		return Serial.readString();
	}
	else
	{
		return String("");
	}

}



// Ativa/desativa o led de debug (led imbutido no arduino)
void Led_Light(boolean Valor) 
{
	if (Valor == true)
	{
		digitalWrite(LED_BUILTIN, HIGH);    
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW); 
	}
  
}



// verifica se o LedON deve estar ativo ou nao . Led do arduino que indica o correto funcionamento do programa
void ExecLedON_Beat() 
{

	gLedON_time_atual = millis();

	gLedON_time_tempo_passado = gLedON_time_atual - gLedON_time_inicio;

	gLedON_SegundosPassados = floor(gLedON_time_tempo_passado / 1000);

	//LogTerm(gLedON_SegundosPassados);

	if (gLedON_Last_SegundosPassados != gLedON_SegundosPassados)
	{
		//LogTerm(gLedON_time_tempo_passado);
	}


	if (gLedON_time_tempo_passado >= ctLED_ON_TEMPO)
	{

		gLedON_time_inicio = millis();

		Led_Light(!gLedON_EstadoAtual);

		gLedON_EstadoAtual = !gLedON_EstadoAtual;

		//LogTerm(gLedON_EstadoAtual);
	}

	gLedON_Last_SegundosPassados = gLedON_SegundosPassados;
  
}





String FormatComZerosAesq(int ValorInt)
{
	if ((ValorInt >= 0) && (ValorInt <= 9)) 
	{
		return String(F("00")) + ValorInt;
	}

	if ((ValorInt >= 10) && (ValorInt <= 99)) 
	{
		return String(F("0")) + ValorInt;
	}

	if ((ValorInt >= 100) && (ValorInt <= 999)) 
	{
		return String(F("")) + ValorInt;
	}

}


    
 
        
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }

    if (found > index)
    {
    	return data.substring(strIndex[0], strIndex[1]);
    }
    else
    {
    	return String("");
    }

    //return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String CharFromAsc2(int Asc2Value)
{
    char ret;

    ret = Asc2Value;

    return String(ret);
}

// retorna a cor em int16 a partir de rgb
uint16_t rgb565_from_triplet(uint8_t red, uint8_t green, uint8_t blue)
{
  red   >>= 3;
  green >>= 2;
  blue  >>= 3;
  return (red << 11) | (green << 5) | blue;
}

