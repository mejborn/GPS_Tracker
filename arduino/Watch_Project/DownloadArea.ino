int8_t answer;
int onModulePin = 2, aux;
int data_size = 0;
int end_file = 0;

char aux_str[100];

char data[250];
int x = 0;
long previous;

char url[] = "sidestreet.dk/arduino/get.php?ID=1";
int port = 80;
char request[] = "";

void requestUrl(){
	// request the url
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
	answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

	if (answer == 1)
	{
		gprs.println(request);
		// Sends <Ctrl+Z>
		aux_str[0] = 0x1A;
		aux_str[1] = 0x00;
		answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 60000);

		x = 0;
		do{
			if (answer == 1)
			{
				data_size = 0;
				while (gprs.available() == 0);
				aux = gprs.read();
				do{
					data_size *= 10;
					data_size += (aux - 0x30);
					while (gprs.available() == 0);
					aux = gprs.read();
				} while (aux != 0x0D);

				gprs.print("Data received: ");
				gprs.println(data_size);

				if (data_size > 0)
				{
					while (gprs.available() < data_size);
					gprs.read();

					for (int y = 0; y < data_size; y++)
					{
						data[x] = gprs.read();
						x++;
					}
					data[x] = '\0';
				}
				else
				{
					gprs.println("Download finished");
				}
			}
			else
			{
				gprs.println("Error getting the url");
				data_size = 0;
			}

			answer = sendATcommand2("", "+CHTTPACT: DATA,", "+CHTTPACT:0", 20000);

		} while (answer != 1);

		if (answer == 2)
		{
			gprs.print("Data received:");
			gprs.println(data);
		}
		else
		{
			gprs.println("Error getting data");
		}
	}
	else
	{
		gprs.println("Error waiting the request");
	}

	delay(10000);
}

void power_on(){

	uint8_t answer = 0;

	// checks if the module is started
	answer = sendATcommand("AT", "OK", 2000);
	if (answer == 0)
	{
		// power on pulse
		digitalWrite(onModulePin, HIGH);
		delay(3000);
		digitalWrite(onModulePin, LOW);

		// waits for an answer from the module
		while (answer == 0){
			// Send AT every two seconds and wait for the answer
			answer = sendATcommand("AT", "OK", 2000);
		}
	}

}

int8_t sendATcommand(char* ATcommand, char* expected_answer1,
	unsigned int timeout)
{

	uint8_t x = 0, answer = 0;
	char response[100];
	unsigned long previous;

	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	while (gprs.available() > 0) gprs.read();    // Clean the input buffer

	gprs.println(ATcommand);    // Send the AT command 


	x = 0;
	previous = millis();

	// this loop waits for the answer
	do{

		if (gprs.available() != 0){
			response[x] = gprs.read();
			x++;
			// check if the desired answer is in the response of the module
			if (strstr(response, expected_answer1) != NULL)
			{
				answer = 1;
			}
		}
		// Waits for the asnwer with time out
	} while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1,
	char* expected_answer2, unsigned int timeout)
{

	uint8_t x = 0, answer = 0;
	char response[100];
	unsigned long previous;

	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	while (gprs.available() > 0) gprs.read();    // Clean the input buffer

	gprs.println(ATcommand);    // Send the AT command 


	x = 0;
	previous = millis();

	// this loop waits for the answer
	do{

		if (gprs.available() != 0){
			response[x] = gprs.read();
			x++;
			// check if the desired answer is in the response of the module
			if (strstr(response, expected_answer1) != NULL)
			{
				answer = 1;
			}
			// check if the desired answer is in the response of the module
			if (strstr(response, expected_answer2) != NULL)
			{
				answer = 2;
			}
		}
		// Waits for the asnwer with time out
	} while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}