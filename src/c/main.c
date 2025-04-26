#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

Vehicle *v_arr = NULL;
Client *c_arr = NULL;
int v_arr_len = 0, c_arr_len = 0;


int main(int argc, char **argv){
	puts("****VEHICLE GENERATION INTERFACE****");
	puts("****PUT -1 AS LICENSE PLATE TO TERMINATE****");
	while (1) {
		printf("****VEHICLE %d****\n", v_arr_len + 1);
		Vehicle v = Vehicle_init_interactive();
		if (v.lp < 0)
			break;
		++v_arr_len;
		v_arr = realloc(v_arr, v_arr_len * sizeof (Vehicle));
		v_arr[v_arr_len - 1] = v;
	} 

	while (1) {
		int choice;

		CLS;

		puts("****MENU****");
		puts("1. RENT VEHICLE");
		puts("2. RETURN VEHICLE");
		puts("3. SHOW VEHICLE STATE");
		puts("4. SHOW PARKING LOT STATE");
		puts("5. SHOW AND STORE AVAILABLE VEHICLES");
		puts("6. SHOW AND STORE RENTED VEHICLES");
		puts("0. END");

		_Client_arr_show(c_arr, c_arr_len);

		INT_INPUT("Choice: ", choice, NO_CONDITION);
		
		switch (choice) {
			case OPTION_RENT_VEHICLE:
				Vehicle_rent(v_arr, c_arr, &v_arr_len, &c_arr_len);
				break;
			case OPTION_RETURN_VEHICLE:
				Vehicle_return(v_arr, c_arr, &v_arr_len, &c_arr_len);
				break;
			case OPTION_SHOW_VEHICLE_STATE:
				Vehicle_show_state(v_arr, v_arr_len);
				break;
			case OPTION_SHOW_PARKING_LOT_STATE:
				Vehicle_show_parking_lot_state(v_arr, v_arr_len);
				break;
			case OPTION_SHOW_AND_STORE_AVAILABLE_VEHICLES:
				Available_vehicle_LL(v_arr);
				break;
			case OPTION_SHOW_AND_STORE_RENTED_VEHICLES:
				Rented_vehicle_LL(v_arr);
				break;
			case OPTION_EXIT:
				puts("Exiting...");
				return EXIT_SUCCESS;
				break;
		}
		_pause();
	}

	return EXIT_SUCCESS;
}
