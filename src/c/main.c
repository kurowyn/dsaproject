#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

Vehicle v_arr[MAX_ARR_LEN];
Client c_arr[MAX_ARR_LEN];
int v_arr_len = 0, c_arr_len = 0;
VehicleNode *v_head_rented, *v_head_available;

int main(int argc, char **argv){
	puts("****INTERFACE SAISI DES VOITURES****");
	puts("(le saisi arrete lorsque vouz sasiez -1 comme matricule).");
	while (1) {
		printf("****VOITURE %d****\n", v_arr_len + 1);
		Vehicle v = Vehicle_init_interactive();
		if (v.lp < 0 || v_arr_len >= 50)
			break;
		++v_arr_len;
		v_arr[v_arr_len - 1] = v;
	} 

	while (1) {
		int choice;

		CLS;

		MAIN_MENU;

		INT_INPUT("Choix: ", choice, NO_CONDITION);
		
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
				v_head_available = Vehicle_LL(v_arr, v_arr_len, AVAILABLE);
				Vehicle_LL_show(v_head_available);
				break;
			case OPTION_SHOW_AND_STORE_RENTED_VEHICLES:
				v_head_rented = Vehicle_LL(v_arr, v_arr_len, RENTED);
				Vehicle_LL_show(v_head_rented);
				break;
			case OPTION_EXIT:
				puts("Exiting...");
				return EXIT_SUCCESS;
				break;
		}
		_pause();
	}

	// should not reach
	return EXIT_SUCCESS;
}
