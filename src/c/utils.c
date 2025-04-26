#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Now to test these.

int 
Client_seek(Client *c_arr, int len, int id){
	for (; len != -1; --len)
		if (c_arr[len].id == id) 
			return len;
	return -1;
}

int 
Vehicle_seek(Vehicle *v_arr, int len, int lp){
	for (; len != -1; --len)
		if (v_arr[len].lp == lp) 
			return len;
	return -1;
}

int 
Client_vehicle_seek(const Client c, int lp){
	for (int i = 0; i < c.len; ++i) 
		if (c.vehicle_lp_arr[i] == lp)
			return i;
	return -1;
}

void 
Client_delete_vehicle(Client *c, int vehicle_pos){
	memmove(c->vehicle_lp_arr + vehicle_pos, 
			c->vehicle_lp_arr + vehicle_pos + 1,
			(c->len - vehicle_pos - 1) * sizeof (int));
	c->vehicle_lp_arr = realloc(c->vehicle_lp_arr, (c->len - 1) * sizeof (int));
	--c->len;
}

Vehicle
Vehicle_init(char *model, int lp, int mileage, State state){
	Vehicle v = (Vehicle){.lp = lp, 
					 	  .mileage = mileage,
					 	  .state = state};

	strcpy(v.model, model);

	return v;
}

Client 
Client_init(char *first_name, char *last_name, int id, int telephone, 
	   		int *vehicle_lp_arr, int len){
	Client c = {.id = id,
				.telephone = telephone};

	c.len = len;
	c.vehicle_lp_arr = vehicle_lp_arr;

	strcpy(c.first_name, first_name);
	strcpy(c.last_name, last_name);

	return c;
}

Client
Client_init_interactive(void){
	Client c;
	INT_INPUT("ID: ", c.id, c.id < 0);
	STRING_INPUT("First Name: ", c.first_name, NO_CONDITION);
	STRING_INPUT("Last Name: ", c.last_name, NO_CONDITION);
	INT_INPUT("Telephone: ", c.telephone, c.telephone < 0);
	c.vehicle_lp_arr = malloc(0);
	c.len = 0;
	return c;
}

Vehicle
Vehicle_init_interactive(void){
	Vehicle v;
	
	STRING_INPUT("Car Model: ", v.model, NO_CONDITION);
	INT_INPUT("License Plate: ", v.lp, NO_CONDITION);
	INT_INPUT("Mileage: ", v.mileage, !(v.mileage >= 0));
	INT_INPUT("State (1 = rented, 0 = available): ", 
			  v.state, !(v.state == RENTED || v.state == AVAILABLE));

	return v;
}

void
_pause(void){
#ifdef WIN32
	system("pause");
#else 
	printf("\nPress any key to continue...");
	getchar();
#endif
}

void 
Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len){
	// First, get the vehicle's number.
	int lp, vehicle_pos;
	INT_INPUT("Enter a license plate: ", lp, NO_CONDITION);
	vehicle_pos = Vehicle_seek(v_arr, *v_arr_len, lp);
	printf("State: %d\n", v_arr[vehicle_pos].state);

	if (vehicle_pos == -1) {
		printf("ERROR: %d, no such vehicle.", lp);
	} else if (v_arr[vehicle_pos].state == RENTED) {
		printf("ERROR: %d, already rented.", lp);
	} else {
		v_arr[vehicle_pos].state = RENTED;

		Client c = Client_init_interactive();

		printf("f_name: %s", c.first_name);

		int client_pos = Client_seek(c_arr, *c_arr_len, c.id);

		if (client_pos == -1) {
			// Allocate more memory, and update size.
			++(*c_arr_len);
			c_arr = realloc(c_arr,
							(*c_arr_len) * sizeof (Client));
			
			// Get the correct position.
			client_pos = *c_arr_len - 1;

			// Finally, update our array.
			c_arr[client_pos] = c;
		}
		
		++(c_arr[client_pos].len);
		c_arr[client_pos].vehicle_lp_arr = realloc(c_arr[client_pos].vehicle_lp_arr,
								   		   		   c_arr[client_pos].len * 
												   sizeof (int));
		c_arr[client_pos].vehicle_lp_arr[c_arr[client_pos].len - 1] = lp;

		_Client_arr_show(c_arr, *c_arr_len);
	}
}

void _Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len,
				   int lp, Client c){
}
void Vehicle_return(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len){
	int lp, vehicle_pos;

	INT_INPUT("License plate: ", lp, NO_CONDITION);

	vehicle_pos = Vehicle_seek(v_arr, *v_arr_len, lp);

	if (vehicle_pos == -1) {
		printf("ERROR: %d, no such vehicle.", lp);
	} else if (v_arr[vehicle_pos].state == RENTED) {
		v_arr[vehicle_pos].state = AVAILABLE;

		int new_mileage;
		INT_INPUT("Add new mileage: ", new_mileage, new_mileage < 0);
		v_arr[vehicle_pos].mileage += new_mileage;

		int id, client_pos;

		INT_INPUT("ID of renting client: ", id, NO_CONDITION);

		client_pos = Client_seek(c_arr, *c_arr_len, id);

		if (client_pos == -1) {
			puts("ERROR: could not find such a client. Attempting to find renting "
				 "client.");
			
			// We look for the client ourselves.
			
			for (int i = 0; i < *c_arr_len; ++i) {
				if (Client_vehicle_seek(c_arr[i], lp) != -1) {
					client_pos = i;
					break;
				}
			}

			// We remove this.

			if (client_pos != -1)
				Client_delete_vehicle(&c_arr[client_pos], 
									  Client_vehicle_seek(c_arr[client_pos], lp));
		} else {
			Client_delete_vehicle(&c_arr[client_pos],
								  Client_vehicle_seek(c_arr[client_pos], lp));
		}
	}
}

void Vehicle_show_state(const Vehicle *v_arr, int v_arr_len){
	int lp, vehicle_pos;
	INT_INPUT("License plate: ", lp, NO_CONDITION);
	vehicle_pos = Vehicle_seek(v_arr, v_arr_len, lp);
	if (vehicle_pos == -1) {
		printf("ERROR: %d, no such vehicle.", lp);
	} else {
		printf("Model: %s\nLicense plate: %d\nMileage: %d\nState: %s",
				v_arr[vehicle_pos].model, v_arr[vehicle_pos].lp,
				v_arr[vehicle_pos].mileage, 
				(v_arr[vehicle_pos].state == RENTED) ? "Rented" : "Available");
	}
}

void Vehicle_show_parking_lot_state(const Vehicle *v_arr, int v_arr_len){
	printf("Total number of vehicles: %d\n", v_arr_len);

	int rented_count = 0, available_count = 0;
	double average_mileage = 0;

	for (int i = 0; i < v_arr_len; ++i) {
		rented_count += v_arr[i].state == RENTED;
		available_count += v_arr[i].state == AVAILABLE;
		average_mileage += v_arr[i].mileage;
	}
	average_mileage = average_mileage / v_arr_len;

	printf("Total number of rented vehicles: %d\n", rented_count);
	puts("License plates of rented vehicles: ");
	for (int i = 0; i < v_arr_len; ++i) 
		if (v_arr[i].state == RENTED)
			printf("%d\n", v_arr[i].lp);

	printf("Total number of available vehicles: %d\n", available_count);
	puts("License plates of available vehicles: ");
	for (int i = 0; i < v_arr_len; ++i) 
		if (v_arr[i].state == AVAILABLE)
			printf("%d\n", v_arr[i].lp);
}

void Available_vehicle_LL(){}

void Rented_vehicle_LL(){}


void 
_Client_arr_show(Client *c_arr, int c_arr_len){
	for (int i = 0; i < c_arr_len; ++i) {
		printf("CLIENT %d\nID: %d\nFirst Name: %s\nLast Name: %s\nTelephone: %d\n",
				i, c_arr[i].id, c_arr[i].first_name, c_arr[i].last_name,	
				c_arr[i].telephone);
		/*
		printf("LICENSE PLATES OF VEHICLES RENTED BY CLIENT %d\n", i);
		for (int j = 0; j < c_arr[i].len; ++j){
			printf("%d\n", c_arr[i].vehicle_lp_arr[j]);
		}
		*/
	}
}
