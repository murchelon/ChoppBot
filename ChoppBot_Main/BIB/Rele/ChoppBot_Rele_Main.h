




// Interrupt is called once a millisecond, looks for any Flow_Pulses_1 from the sensor!
/*

SIGNAL(TIMER0_COMPA_vect) 
{
	uint8_t x = digitalRead(FLOW_PINO_SENSOR_1);

	if (x == lastflowpinstate_1)
	{
		lastflowratetimer_1++;
		return; // nothing changed!
	}

	if (x == HIGH) 
	{
		//low to high transition!
		Flow_Pulses_1++;
	}

	lastflowpinstate_1 = x;
	flowrate_1 = 1000.0;
	flowrate_1 /= lastflowratetimer_1;  // in hertz
	lastflowratetimer_1 = 0;

	
}

*/


SIGNAL(TIMER1_COMPA_vect)
{
  
	uint8_t x1 = digitalRead(gPinoSensorFluxoAtivo);

	if (x1 == gLastflowpinstate_Atual) 
	{
		gLastflowratetimer_Atual++;
		return; // nothing changed!
	}


	if (x1 == HIGH) 
	{
		//low to high transition!
		gFlow_Pulses_Atual++;
		gFlow_Pulses_Corrigido_Atual++;
	}

	//Flow_Pulses_Corrigido_2 = Flow_Pulses_2;

	gLastflowpinstate_Atual = x1;
	gFlowrate_Atual = 1000.0;
	gFlowrate_Atual /= gLastflowratetimer_Atual;  // in hertz
	gLastflowratetimer_Atual = 0;

	//LogTerm(String(F("gFaixaVelAtual = ")) + String(gFaixaVelAtual));

	if (gFaixaVelAtual == -4)
	{
		gFlow_Pulses_Corrigido_Atual = gFlow_Pulses_Corrigido_Atual + 1.5;		
	}

	if (gFaixaVelAtual == 5)
	{
		gFlow_Pulses_Corrigido_Atual = gFlow_Pulses_Corrigido_Atual - 0.9;		
	}

	//if (gFaixaVelAtual == -4)
	//{
	//	Flow_Pulses_Corrigido_2 = Flow_Pulses_Corrigido_2 + 5;		
	//}

}
/*


SIGNAL(TIMER2_COMPA_vect) 
{
  
	uint8_t x2 = digitalRead(FLOW_PINO_SENSOR_3);

	if (x2 == lastflowpinstate_3) 
	{
		lastflowratetimer_3++;
		return; // nothing changed!
	}


	if (x2 == HIGH)
	{
		//low to high transition!
		Flow_Pulses_3++;
	}


	lastflowpinstate_3 = x2;
	flowrate_3 = 1000.0;
	flowrate_3 /= lastflowratetimer_3;  // in hertz
	lastflowratetimer_3 = 0;
}

*/

// void useInterrupt________________________________________

/*
void useInterrupt_1(boolean v) 
{
	if (v) 
	{
		// Timer0 is already used for millis() - we'll just interrupt somewhere
		// in the middle and call the "Compare A" function above
		OCR0A = 0xAF;
		TIMSK0 |= _BV(OCIE0A);
	} 
	else 
	{
		// do not call the interrupt function COMPA anymore
		TIMSK0 &= ~_BV(OCIE0A);
	}
}

*/

void useInterrupt_2(boolean c) 
{
	if (c)
	{
		// Timer0 is already used for millis() - we'll just interrupt somewhere
		// in the middle and call the "Compare A" function above
		OCR1A = 0xAF;
		TIMSK1 |= _BV(OCIE1A);
	} 
	else 
	{
		// do not call the interrupt function COMPA anymore
		TIMSK1 &= ~_BV(OCIE1A);
	}
} 


/*
void useInterrupt_3(boolean b) 
{
	if (b) 
	{
		// Timer0 is already used for millis() - we'll just interrupt somewhere
		// in the middle and call the "Compare A" function above
		OCR2A = 0xAF;
		TIMSK2 |= _BV(OCIE2A);
	} 
	else 
	{
		// do not call the interrupt function COMPA anymore
		TIMSK2 &= ~_BV(OCIE2A);
	}
} 
*/

void TELA_Render_Interface_OPERACAO_SERVICO()
{
	

	gFlow_Pulses_Corrigido_Atual = 0;
	gFlow_Pulses_Atual = 0;

    String tmp_IDChopp = F("");
    String tmp_Nome = F("");
    String tmp_Tipo = F("");
    String tmp_Valor = F("");
    String tmp_Volume = F("");
    String tmp_Ativa = F("");		

    float liters_Atual = 0;

	float ValorSessaoChopp = 0;
	float ValorSaldoAtual = 0;
	float VolumeAtual = 0;


	if (gTelaRenderizada_OPERACAO_SERVICO == false)
	{

        //LogTerm(F("== [Modo Atual: STANDBY] ==");

		// Liga Led Verde
		LED_SetLedState("RGB", true, "GREEN");



		//ValorSaldoAtual = 0;
		//ValorSessaoChopp = 0;
		//tmp_Valor = 0;
		//liters_Atual = 0;
		//VolumeAtual = 0;



		switch (gServico_ID_TorneiraAtual)
		{

			case 1:

				gPinoSensorFluxoAtivo = FLOW_PINO_SENSOR_1;

				gPinoReleTorneiraAtiva = RELE_PINO_TORNEIRA_1;	

				break;

			case 2:

				gPinoSensorFluxoAtivo = FLOW_PINO_SENSOR_2;

				gPinoReleTorneiraAtiva = RELE_PINO_TORNEIRA_2;

				break;

			case 3:

				gPinoSensorFluxoAtivo = FLOW_PINO_SENSOR_3;

				gPinoReleTorneiraAtiva = RELE_PINO_TORNEIRA_3;	

				break;

			case -1:
				// nenhuma torneira atuva no momento
			
			
			default:
				break;				
		}


		// Inicia o FLOW METERS
		digitalWrite(gPinoSensorFluxoAtivo, HIGH);
		gLastflowpinstate_Atual = digitalRead(gPinoSensorFluxoAtivo);

		useInterrupt_2(true);

		//if (gServico_ID_TorneiraAtual == 1) { useInterrupt_1(true); }
		//if (gServico_ID_TorneiraAtual == 2) { useInterrupt_2(true); }
		//if (gServico_ID_TorneiraAtual == 3) { useInterrupt_3(true); }




		/*
		// Inicia os FLOW METERS
		digitalWrite(FLOW_PINO_SENSOR_1, HIGH);
		lastflowpinstate_1 = digitalRead(FLOW_PINO_SENSOR_1);

		digitalWrite(FLOW_PINO_SENSOR_2, HIGH);
		lastflowpinstate_2 = digitalRead(FLOW_PINO_SENSOR_2);

		digitalWrite(FLOW_PINO_SENSOR_3, HIGH);
		lastflowpinstate_3 = digitalRead(FLOW_PINO_SENSOR_3);

		useInterrupt_1(true);
		useInterrupt_2(true);
		useInterrupt_3(true);
		*/




		// Recupera os valores para a torneira desejada

		/*
	    // NumTorneira;DataCad;IDChopp;VolumeAtual;DataExpira;Ativa;NomeFromBanco;Tipo;Valor
	    for (int x = 0 ; x <= ctMAX_TORNEIRAS ; x++)
	    {

	        if (gaEngatados[x] != "")
	        {


	            String tmp_IDChopp = getValue(gaEngatados[x], ';', 2);
	            String tmp_Nome = getValue(gaEngatados[x], ';', 7);
	            String tmp_Tipo = getValue(gaEngatados[x], ';', 8);
	            String tmp_Valor = getValue(gaEngatados[x], ';', 9);
	            String tmp_Volume = getValue(gaEngatados[x], ';', 3);
	            //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
	            //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
	            String tmp_Ativa = getValue(gaEngatados[x], ';', 5);

	        }


	    }
		*/



        tmp_IDChopp = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 2);
        tmp_Nome = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 7);
        tmp_Tipo = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 8);
        tmp_Valor = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 9);
        tmp_Volume = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 3);
        //tmp_DataCad = getValue(gaEngatados[x], ';', 1);
        //tmp_DataExp = getValue(gaEngatados[x], ';', 4);
        tmp_Ativa = getValue(gaEngatados[gServico_ID_TorneiraAtual - 1], ';', 5);



		//////////////////////////////////////  
		// cebcalho logado

		if (ctTELA_HARDWARE == F("TERMINAL"))
		{  

			LogTerm(String(F("Nome: ")) + gSessao_Nome);
			LogTerm(String(F("Saldo: ")) + FormatNumber(gSessao_SaldoAtual, F("MONEY")));
			LogTerm(String(F("Tempo: ")) + String(ctTIMEOUT_TORNEIRA / 1000) + String(F(" segundos")));
			LogTerm(String(F("Torneira ")) + String(gServico_ID_TorneiraAtual) + String(F(" liberada!")));

			LogTerm(String(F("Chopp: ")) + tmp_Nome);
			LogTerm(String(F("Tipo: ")) + tmp_Tipo);
			LogTerm(String(F("Preco: ")) + FormatNumber(tmp_Valor.toFloat(), F("MONEY")) + String(F(" / Litro")));
			LogTerm(String(F("Restante: ")) + tmp_Volume.toFloat() + String(F(" Litros")));
			LogTerm(F("Volume Retirado: 0,00 Litros"));
			LogTerm(String(F("Valor do Chopp sendo retirado: ")) + FormatNumber(0.0, F("MONEY")));

			LogTerm(F("Pode se servir..."));

		}

		if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
		{  



			TELA_SetFontSize(1); 

			tft.setTextColor(CinzaLabels);
			tft.setCursor (10, 10);			
			tft.print (F("Nome: ")); 
			tft.setCursor (10, 45);			
			tft.print (F("Saldo: "));  

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (120, 10);			
			tft.print (gSessao_Nome); 
			tft.setCursor (120, 45);	
			tft.print (FormatNumber(gSessao_SaldoAtual, F("MONEY")));  


			//////////////////////////////////////  
			// Tempo Restante

			TELA_SetFontSize(0); 
			tft.setTextColor(CinzaLabels);
			tft.setCursor (620, 10);			
			tft.print (F("Tempo: ")); 

			tft.setCursor (680, 10);
			tft.print (String(ctTIMEOUT_TORNEIRA / 1000) + String(F(" segundos   "))); 


			//////////////////////////////////////  
			// texto torneira liberada

			TELA_SetFontSize(2); 
			tft.setTextColor(VerdeOK);

			tft.setCursor (170, 110);			
			tft.print (String(F("Torneira ")) + String(gServico_ID_TorneiraAtual) + String(F(" liberada!")));    

			tft.setCursor (220, 160);			
			tft.print (F("Pode se servir..."));    

			//////////////////////////////////////

			TELA_SetFontSize(1); 

			//////////////////////////////////////
			// CHOPP

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 240);			
			tft.print (F("Chopp: ")); 

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (340, 240);			
			tft.print (tmp_Nome);


			//////////////////////////////////////
			// TIPO

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 270);			
			tft.print (F("Tipo: ")); 

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (340, 270);			
			tft.print (tmp_Tipo);


			//////////////////////////////////////
			// PRECO

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 300);			
			tft.print (F("Preco: ")); 

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (340, 300);			
			tft.print (FormatNumber(tmp_Valor.toFloat(), F("MONEY")) + String(F(" / Litro"))); 



			//////////////////////////////////////
			// RESTANTE

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 330);			
			tft.print (F("Restante: ")); 

			tft.setTextColor(RA8875_WHITE);
			tft.setCursor (340, 330);			
			tft.print (tmp_Volume.toFloat() + String(F(" Litros")));




			//////////////////////////////////////
			// VOLUME RETIRADO

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 390);			
			tft.print (F("Volume Retirado: ")); 

			tft.setTextColor(RA8875_YELLOW);
			tft.setCursor (450, 390);			
			tft.print ("0,00 Litros");

			//////////////////////////////////////
			// VALOR DO CHOPP SENDO RETIRADO

			tft.setTextColor(CinzaLabels);
			tft.setCursor (180, 420);			
			tft.print (F("Valor do Chopp: ")); 

			tft.setTextColor(RA8875_YELLOW);
			tft.setCursor (450, 420);			
			tft.print (FormatNumber(0.0, F("MONEY"))); 


		}







		////////////////////////////////////////////////////////////////////////////////////
		//ROTINA DE LIBERACAO DO CHOPP
		////////////////////////////////////////////////////////////////////////////////////

		boolean Exec_Loop_PodeSair = false;

		unsigned long time_inicio;
		unsigned long time_atual;
		unsigned long time_tempo_passado;

		int SegundosPassados;
		int Last_SegundosPassados;




		time_inicio = millis();

		//TELA_Texto(String(F("MODO OPERACAO SERVICO: Saindo em ")) + String(ctTIMEOUT_TORNEIRA / 1000) + String(F(" segundos...")), F("BRANCO"));


	    // LIBERA TORNEIRA
	    //digitalWrite(RELE_PINO_TORNEIRA_2, LOW);
		digitalWrite(gPinoReleTorneiraAtiva, LOW);

		
		while (Exec_Loop_PodeSair == false)
		{


		    time_atual = millis();
		    time_tempo_passado = time_atual - time_inicio;

		    SegundosPassados = floor(time_tempo_passado / 1000);



		    if (SegundosPassados != Last_SegundosPassados)
		    {
		        //LogTerm(SegundosPassados);
		        //TELA_Texto(String(SegundosPassados), F("AMARELO"));


	
				if (ctTELA_HARDWARE == F("TERMINAL"))
				{  

					LogTerm(String((ctTIMEOUT_TORNEIRA / 1000) - SegundosPassados) + String(F(" segundos   ")));

				}

				if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
				{  
				
					TELA_SetFontSize(0); 
					tft.setTextColor(CinzaLabels, RA8875_BLACK);
					tft.setCursor (680, 10);			
					tft.print (String((ctTIMEOUT_TORNEIRA / 1000) - SegundosPassados) + String(F(" segundos   "))); 	

				}


				

		    }



		    if (time_tempo_passado >= ctTIMEOUT_TORNEIRA)
		    {
		        Exec_Loop_PodeSair = true;
		    }

		    Last_SegundosPassados = SegundosPassados;



		    

		   	if (gFLOW_PulsosNosUltimosXseg >= ctLIMITE_IGNORA_PULSOS)
		   	{

				// FLOW METER____________________________________________________


				liters_Atual = gFlow_Pulses_Corrigido_Atual;
				liters_Atual /= gAjusteFluxo;
				liters_Atual /= 60.0;


			}
		    /*
			float liters_1 = (float)Flow_Pulses_1;
			liters_1 /= gAjusteFluxo;
			liters_1 /= 60.0;

			float liters_2 = Flow_Pulses_Corrigido_2;
			liters_2 /= gAjusteFluxo;
			liters_2 /= 60.0;

			float liters_3 = (float)Flow_Pulses_3;
			liters_3 /= gAjusteFluxo;
			liters_3 /= 60.0;
			*/


			//  Serial.println(level);
			//  Serial.print(liters); 
			//  Serial.print(" Litros"); 
			//  Serial.print("        "); 
			//  Serial.print(liters2); 
			//  Serial.println(" Liters");

			delay(100);






			if (gFLOW_Fluxo_Iniciado == true)
			{

				if (gFLOW_Reinicia_Contador == true)
				{
					gFLOW_time_inicio = millis();
					gFLOW_Reinicia_Contador = false;
					gFLOW_PulsosNoInicio = gFlow_Pulses_Atual;
					//SegundosPassados = 0;
					//time_tempo_passado = 0;

					

				}

				gFLOW_time_atual = millis();
				gFLOW_time_tempo_passado = gFLOW_time_atual - gFLOW_time_inicio;
				//SegundosPassados = floor(time_tempo_passado / 1000);
				gFLOW_SegundosPassados = gFLOW_time_tempo_passado / 1000;

				//LogTerm("Pulsos atuais = " + String(Flow_Pulses_3) + " | gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg));
				//LogTerm("gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg));
				//LogTerm("gFLOW_PulsosNoInicio = " + String(gFLOW_PulsosNoInicio));
				//LogTerm("gFLOW_time_atual = " + String(gFLOW_time_atual));
				//LogTerm("gFLOW_time_inicio = " + String(gFLOW_time_inicio));
				//LogTerm("gFLOW_time_tempo_passado = " + String(gFLOW_time_tempo_passado));
				//LogTerm("gFLOW_SegundosPassados = " + String(gFLOW_SegundosPassados));



				if (gFLOW_time_tempo_passado >= 300)
				{

					gFLOW_PulsosNosUltimosXseg = gFlow_Pulses_Atual - gFLOW_PulsosNoInicio;

					//LogTerm("Numero de Pulsos nos ultimos 1 segundos: = " + String(gFLOW_PulsosNosUltimosXseg));


					gFLOW_Reinicia_Contador = true;
					//time_inicio = millis();
					//PulsosNoInicio = Flow_Pulses_3;
					//SegundosPassados = 0;
					//time_tempo_passado = 0;
				}




				delay(400);

			}





			if (gFLOW_PulsosNosUltimosXseg >= ctLIMITE_IGNORA_PULSOS)
			{
				time_inicio = millis();
			}


			//time_inicio = millis();


			gFaixaVelAtual = 99;


			#define Range_Menos 10
			#define Range_Mais 10

			LogTerm(String(F("gFLOW_PulsosNosUltimosXseg = ")) + String(gFLOW_PulsosNosUltimosXseg));
			LogTerm(String(F("gFlow_Pulses_Corrigido_Atual = ")) + String(gFlow_Pulses_Corrigido_Atual));



			// FAIXA IDEAL = 0
			if ((gFLOW_PulsosNosUltimosXseg >= gPulse_ideal - Range_Menos) && (gFLOW_PulsosNosUltimosXseg <= gPulse_ideal - Range_Mais))
			{
				gFaixaVelAtual = 0;
			}


			/*
			// velocidade ideal
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal - (gAjuste_fino_baixo * 1))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal + (gAjuste_fino_baixo * 1))))
			{
				gFaixaVelAtual = 0;
				//LogTerm("IDEAL -- gFLOW_PulsosNosUltimosXseg: " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal - (gAjuste_fino_baixo * 1))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 1)) + " = IDEAL | Pulsos: " + String(gFlow_Pulses_Atual));
			}


			// velocidade -1
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal - (gAjuste_fino_baixo * 2))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal - (gAjuste_fino_baixo * 1))))
			{
				gFaixaVelAtual = -1;
				//LogTerm("VEL -1: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal - (gAjuste_fino_baixo * 2))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 1)) + " = -1 | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			// velocidade +1
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal + (gAjuste_fino_baixo * 1))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal + (gAjuste_fino_baixo * 2))))
			{
				gFaixaVelAtual = 1;
				//LogTerm("VEL +1: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal + (gAjuste_fino_baixo * 1))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 2)) + " = +1 | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			// velocidade -2
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal - (gAjuste_fino_baixo * 3))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal - (gAjuste_fino_baixo * 2))))
			{
				gFaixaVelAtual = -2;
				//LogTerm("VEL -2: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal - (gAjuste_fino_baixo * 3))) + " ate " + (gPulse_ideal - (gAjuste_fino_baixo * 2)) + " = -2 | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			// velocidade +2
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal + (gAjuste_fino_baixo * 2))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal + (gAjuste_fino_baixo * 3))))
			{
				gFaixaVelAtual = 2;
				//LogTerm("VEL +2: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal + (gAjuste_fino_baixo * 2))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 3)) + " = +2 | Pulsos: " + String(gFlow_Pulses_Atual));
			}


			// velocidade -3
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal - (gAjuste_fino_baixo * 3))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal - (gAjuste_fino_baixo * 2))))
			{
				gFaixaVelAtual = -3;
				//LogTerm("VEL -3: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal - (gAjuste_fino_baixo * 3))) + " ate " + (gPulse_ideal - (gAjuste_fino_baixo * 2)) + " = -3 | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			// velocidade +3
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal + (gAjuste_fino_baixo * 3))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal + (gAjuste_fino_baixo * 4))))
			{
				gFaixaVelAtual = 3;
				//LogTerm("VEL +3: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal + (gAjuste_fino_baixo * 3))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 4)) + " = +3 | Pulsos: " + String(gFlow_Pulses_Atual));
			}


			// velocidade -4
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal - (gAjuste_fino_baixo * 4))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal - (gAjuste_fino_baixo * 3))))
			{
				gFaixaVelAtual = -4;
				//LogTerm("VEL -4: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal - (gAjuste_fino_baixo * 4))) + " ate " + (gPulse_ideal - (gAjuste_fino_baixo * 3)) + " = -4 | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			// velocidade +4
			if ((gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal + (gAjuste_fino_baixo * 4))) && (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal + (gAjuste_fino_baixo * 4))))
			{
				gFaixaVelAtual = 4;
				//LogTerm("VEL +4: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | de " + String((gPulse_ideal + (gAjuste_fino_baixo * 4))) + " ate " + (gPulse_ideal + (gAjuste_fino_baixo * 4)) + " = +4 | Pulsos: " + String(gFlow_Pulses_Atual));
			}


			// velocidade EXTREMA BAIXA
			if (gFLOW_PulsosNosUltimosXseg < (gPulse_ideal - gAjuste_fino_baixo * 5))
			{
				gFaixaVelAtual = -5;
				//LogTerm("VEL EXTREMA BAIXA: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | MENOR que " + String(gPulse_ideal - (gAjuste_fino_baixo * 5)) + " | Pulsos: " + String(gFlow_Pulses_Atual));
			}


			// velocidade EXTREMA ALTA
			if (gFLOW_PulsosNosUltimosXseg >= (gPulse_ideal + gAjuste_fino_baixo * 5))
			{   
				gFaixaVelAtual = 5;
				//LogTerm("VEL EXTREMA ALTA: gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg) + " | MAIOR que " + String(gPulse_ideal + (gAjuste_fino_baixo * 5)) + " | Pulsos: " + String(gFlow_Pulses_Atual));
			}

			*/





			//LogTerm("gFLOW_PulsosNosUltimosXseg = " + String(gFLOW_PulsosNosUltimosXseg));


			// RENDER DOS VALORES ATUALIZADOS EM TELA


			if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
			{  
				TELA_SetFontSize(1);
			}

			

			//////////////////////////////////////
			// VOLUME RETIRADO

			//LogTerm("liters_1 = ");
			//LogTerm(liters_1);

		   	if (gFLOW_PulsosNosUltimosXseg >= ctLIMITE_IGNORA_PULSOS)
		   	{				

				if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
				{  
				
					
					tft.setTextColor(RA8875_YELLOW, RA8875_BLACK);
					tft.setCursor (450, 390);			
					tft.print (String(FormatNumber(liters_Atual, "")) + " Litros     ");

				}

				//////////////////////////////////////
				// VALOR DO CHOPP SENDO RETIRADO



				ValorSessaoChopp = 0;

				ValorSessaoChopp = tmp_Valor.toFloat() * liters_Atual;


				if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
				{  
				
					tft.setTextColor(RA8875_YELLOW, RA8875_BLACK);
					tft.setCursor (450, 420);			
					tft.print (FormatNumber(ValorSessaoChopp, F("MONEY")) + String("     ")); 

				}


				//////////////////////////////////////  
				// cebcalho logado -- SALDO ATUAL

				ValorSaldoAtual = 0;

				ValorSaldoAtual = gSessao_SaldoAtual - ValorSessaoChopp;

				if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
				{  

					tft.setTextColor(RA8875_WHITE, RA8875_BLACK);
					tft.setCursor (120, 45);	
					tft.print (FormatNumber(ValorSaldoAtual, F("MONEY")) + String("     "));  

				}


				//////////////////////////////////////
				// RESTANTE

				VolumeAtual = 0;

				VolumeAtual = tmp_Volume.toFloat() - liters_Atual;

				if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
				{  

					tft.setTextColor(RA8875_WHITE, RA8875_BLACK);
					tft.setCursor (340, 330);			
					tft.print (FormatNumber(VolumeAtual, "") + String(F(" Litros")) + String("     "));

				}




			}




		}

		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////


	
		//aqui

	    // FECHA TORNEIRA -- FINALIZA SESSAO
	    digitalWrite(gPinoReleTorneiraAtiva, HIGH);


		useInterrupt_2(false);
		delay(100);


	    


	    LogTerm(String(F("---------------------------------------")));

	    LogTerm(String(F("Torneira (gServico_ID_TorneiraAtual) = ")) + String(gServico_ID_TorneiraAtual));
	    LogTerm(String(F("IDChopp (tmp_IDChopp) = ")) + tmp_IDChopp);
	    LogTerm(String(F("Nome Chopp (tmp_Nome) = ")) + tmp_Nome);
	    LogTerm(String(F("Preco Chopp (tmp_Valor) = ")) + tmp_Valor);
	    LogTerm(String(F("IDUser = ")) + String(gSessao_IDUser));
		LogTerm(String(F("gSessao_Nome = ")) + gSessao_Nome);
		LogTerm(String(F("gSessao_CPF = ")) + gSessao_CPF);
		LogTerm(String(F("gSessao_DataCad = ")) + gSessao_DataCad);
		LogTerm(String(F("Saldo Original (gSessao_SaldoAtual) = ")) + String(gSessao_SaldoAtual));
		LogTerm(String(F("Pulsos (gFlow_Pulses_Corrigido_Atual) = ")) + String(gFlow_Pulses_Corrigido_Atual));
		LogTerm(String(F("Consumido (liters_Atual) = ")) + String(liters_Atual));
		LogTerm(String(F("ValorSessaoChopp = ")) + String(ValorSessaoChopp));
		LogTerm(String(F("Saldo Atual (ValorSaldoAtual) = ")) + String(ValorSaldoAtual));		
		LogTerm(String(F("Chopp Restante (VolumeAtual) = ")) + String(VolumeAtual));

		LogTerm(String(F("---------------------------------------")));



		String retFunc = F("");
		retFunc = BANCO_AtualizaSaldoUserLogado(tmp_IDChopp,
		                                        tmp_Nome,
		                                        tmp_Valor,
		                                        liters_Atual,
		                                        ValorSessaoChopp,
		                                        ValorSaldoAtual,
		                                        VolumeAtual
		                                        );

		if (retFunc.substring(0, 1) == F("1"))
		{
		    LogTerm(F("Saldo/Sessao de usuario atualizado com sucesso !"));
		}
		else
		{
		    LogTerm(String(F("Falha na atualizacao do Saldo/Sessao: ")) + retFunc);
		}

		

		volatile unsigned long gBounce_time_inicio = 0;
		volatile unsigned long gBounce_time_atual = 0;
		volatile unsigned long gBounce_time_tempo_passado = 0;

		volatile int gBounce_SegundosPassados = 0;
		volatile int gBounce_Last_SegundosPassados = 0;


		// var que conta o numero de cliques executaados no botao
		volatile int gBounce_ContaClick = 0;	    


		gServico_ID_TorneiraAtual = -1;	

		tmp_IDChopp = F("");
		tmp_Nome = F("");
		tmp_Valor = F("");
		
		gFlow_Pulses_Corrigido_Atual = 0;
		gFlow_Pulses_Atual = 0;
		liters_Atual = 0;


		ValorSessaoChopp = 0;
		ValorSaldoAtual = 0;
		VolumeAtual = 0;

		gPinoSensorFluxoAtivo = -1;	
		gPinoReleTorneiraAtiva = -1;



		// zera as vars para cada tentativa de login
		// efetua logoff
		gSessao_Logado = false;
		gSessao_IDUser = -1;
		gSessao_Nome = F("");
		gSessao_Nivel = -1;
		gSessao_SaldoAtual = -1;
		gSessao_CPF = F("");
		gSessao_DataCad = F("");
		
		gTelaRenderizada_OPERACAO_SERVICO = false;
		gTelaRenderizada_OPERACAO = false;	

		gModoOperacao = F("STANDBY");  
		gModoOperacao_SubTela = F("");	



		delay(500); 

		LED_SetLedState("RGB", false, "");


	    if (ctTELA_HARDWARE == String(F("ER-TFTM070-5")))
	    {
	        TELA_LimpaTela();
	    }




	}
	

}



void TELA_VerificaTouch_OPERACAO_SERVICO()
{

	if (String(ctTELA_HARDWARE) == String(F("ER-TFTM070-5")))
	{  



		//tft.changeMode(GRAPHIC);

		if (TELA_touchDetect())
		{

			TELA_touchReadPixel(&gTouch_X, &gTouch_Y);


			//TELA_LogTerm_XY(); 

			//botao 1:
			if (gTouch_X >= 100 && gTouch_X <= 100 + 170)  
			{

				if (gTouch_Y >= 250 && gTouch_Y <= 250 + 60) 
				{



				}

			}




		}

	}

}


