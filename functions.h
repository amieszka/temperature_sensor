
/* function returning decimal value from binary digits */
double binary_to_decimal(char temp_value[], int b_num) {
  double decimal = 0; //stores decimal value
  int temperature[b_num]; //stores array of bytes as integers
  int neg = 0; // 0 if positive, 1 if negative
  int k = 0; // number of bytes to change while adding 1

  /* loop reversing data from file to temperature array */
  for (int n = 0; n<b_num; n++) {
    int byte = temp_value[n] - '0'; 
    temperature[b_num - 1 - n] = byte;
  }

  /* loop checking if negative, if so, complementing binary number and adding one */
  if ((temperature[0] == 1) && (temperature[1] == 1)) {
    neg = 1;
    /* loop complemening binary number */
    for (int n = 0; n<b_num; n++) {
      if (temperature[n] == 0) {
        temperature[n] = 1;
      }
      else {
        temperature[n] = 0;
      }
    }
    /* loop checking where to add 1 */
    for (int n = 0; n<b_num; n++) {
      if (temperature[b_num - 1 - n] == 0) {
        k = n;
        break;
      }
    }
    /* loop adding 1 */
    temperature[b_num - 1 - k] = 1;
    for (int j = 0; j < k; j++) {
      temperature[b_num - 1 - j] = 0;
    }
  }

  /* loop changing binary table of int to decimal int value */
  for (int n=0; n<b_num; n++) {
    decimal = decimal + temperature[n]*pow(2,b_num - 1 - n);
  }

  /* condition giving negative number minus sign */
  if (neg == 1) {
    decimal = - decimal;
  }
  return decimal;
}


/* function reading binary file data*/
int read_file() {
  char patch; // stores patch name
  FILE * file;
  size_t len; 
  char * line = (char *)malloc(len); // stores particular line from file 
  int read;
  
  char buffer_temp[8*sizeof(int16_t)]; // buffer to store temperature bytes from file
  char buffer_ID[8*sizeof(uint16_t)]; // buffer to store sensor ID bytes from file
  
  int lower = -40; // minimal value of temperature range
  int upper = 80; // maximal value of temperature range
  float resolution = 0.015625; // resolution of sensor
  int T_from = 8*(2*sizeof(uint32_t) + sizeof(uint8_t)) + 1; // stores value where temperature bytes starts
  int T_to = 8*(2*sizeof(uint32_t) + sizeof(uint8_t) + sizeof(int16_t)); // stores value where temperature bytes endss
  int ID_from = 8*(sizeof(uint32_t) + sizeof(uint8_t)); // stores value where sensor ID bytes starts
  int ID_to = 8*(2*sizeof(uint32_t) + sizeof(uint8_t)); // stores value where sensor ID bytes ends

  printf("Enter filename:");
  scanf("%s", &patch);
  
  file = fopen(&patch,"rb");
  
  /* if file exists do: */
  if (file != NULL) {

    /* loop reading file line by line */
    while ((read = getline(&line, &len, file)) != -1) {
      int i = 0, j = 0;
      char c;
      
      /* loop returning temperature bytes to array of chars */
      for (i = 0, j = 0; i <= T_to && c != EOF; i++) {
        if (i >= T_from) { 
          buffer_temp[j] = c;
          j++;
        }
        c = line[i];
      }
      
      double temperature = resolution*binary_to_decimal(buffer_temp, 14); // temperature bytes to decimal value convertion
      printf("Temperature value: %.3f degrees Celcius\n", temperature);

      /* if temperature is out of range */
      if (temperature >= upper || temperature < lower) {
      c = 0;
      /* loop returning sensor ID bytes to array of chars */
      for (i = 0, j = 0; i <= ID_to && c != EOF; i++){
        if (i >= ID_from) {
          buffer_ID[j] = c;
          j++;
        }
        c = line[i];
      }
    
      int sensor_ID = binary_to_decimal(buffer_ID, 32);
      printf("ALERT!: Temperature value of sensor with ID %d is out of range %d; %d\n", sensor_ID, upper, lower); 
    }
      
    buffer_ID[0] = 0;
    buffer_temp[0] = 0;
    line[0] = 0;
    }
  }
  
  /* if file does not exist: */
  else {
    printf("No such file as %s\n", &patch);
    return(1);
  }
  
  free(line);
  fclose(file);

  return(0);
}
