#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void
_pause(void){
#ifdef WIN32
	system("pause");
#else 
	printf("\nPress any key to continue...");
	getchar();
#endif
}

int 
Client_seek(Client *c_arr, int c_arr_len, int id){
	for (; c_arr_len != -1; --c_arr_len)
		if (c_arr[c_arr_len].id == id) 
			return c_arr_len;
	return -1;
}

int 
Vehicle_seek(Vehicle *v_arr, int v_arr_len, int lp){
	for (; v_arr_len != -1; --v_arr_len)
		if (v_arr[v_arr_len].lp == lp) 
			return v_arr_len;
	return -1;
}

int 
Client_vehicle_seek(const Client c, int lp){
	for (int i = 0; i < c.lp_arr_len; ++i) 
		if (c.lp_arr[i] == lp)
			return i;
	return -1;
}

void 
Client_delete_vehicle(Client *c, int vehicle_lp_pos){
	memmove(c->lp_arr + vehicle_lp_pos, 
			c->lp_arr + vehicle_lp_pos + 1,
			(c->lp_arr_len - vehicle_lp_pos - 1) * sizeof (int));
	--c->lp_arr_len;
}

Client
Client_init_interactive(void){
	Client c;
	INT_INPUT("CIN: ", c.id, c.id < 0);
	STRING_INPUT("Nom: ", c.last_name, NO_CONDITION);
	STRING_INPUT("Prenom: ", c.first_name, NO_CONDITION);
	INT_INPUT("Telephone: ", c.telephone, c.telephone < 0);
	c.lp_arr[0] = 0;
	c.lp_arr_len = 0;
	return c;
}

Vehicle
Vehicle_init_interactive(void){
	Vehicle v;
	
	STRING_INPUT("Modele: ", v.model, NO_CONDITION);
	INT_INPUT("Matricule: ", v.lp, NO_CONDITION);
	INT_INPUT("Kilometrage: ", v.mileage, !(v.mileage >= 0));
	INT_INPUT("Etat (1 = loué, 0 = disponible): ", 
			  v.state, !(v.state == RENTED || v.state == AVAILABLE));

	return v;
}

void 
Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len){
	// First, get the vehicle's lp.
	int lp, vehicle_pos;

	INT_INPUT("Matricule: ", lp, NO_CONDITION);

	vehicle_pos = Vehicle_seek(v_arr, *v_arr_len, lp);

	if (vehicle_pos == -1) {
		ERROR_MSG_NOT_FOUND(lp);
	} else if (v_arr[vehicle_pos].state == RENTED) {
		ERROR_MSG_ALREADY_RENTED(lp);
	} else {
		Client c = Client_init_interactive();

		int client_pos = Client_seek(c_arr, *c_arr_len, c.id);

		if (client_pos == -1) {
			// First, check if there's enough space.
			if (*c_arr_len < MAX_ARR_LEN) {
				// In this case, we can add our client.
				++(*c_arr_len);
				c_arr[*c_arr_len - 1] = c;
				
				// Since it's a new client, it shouldn't have already rented
				// vehicles' license plates, but we check for good measure that
				// the number doesn't exceed MAX_ARR_LEN.
				if (c_arr[*c_arr_len - 1].lp_arr_len < MAX_ARR_LEN) {
					// We add it.
					++(c_arr[*c_arr_len - 1].lp_arr_len);
					c_arr[*c_arr_len - 1]
						.lp_arr[c_arr[*c_arr_len - 1].lp_arr_len - 1] = 
						v_arr[vehicle_pos].lp;
					// Here, we mark the vehicle as rent.
					v_arr[vehicle_pos].state = RENTED;
				} else {
					// Throw an error message.
					ERROR_MSG_MAX_RENT_CAPACITY(c_arr[*c_arr_len - 1].id);
				}
			} else {
				ERROR_MSG_MAX_CLIENT_CAPACITY;
			}
		} else {
			// In this case, check our client's number of rented vehicles.
			if (c_arr[client_pos].lp_arr_len < 50) {
				// Add it.
				++(c_arr[client_pos].lp_arr_len);
				c_arr[client_pos].lp_arr[c_arr[client_pos].lp_arr_len - 1] = 
					v_arr[vehicle_pos].lp;
				v_arr[vehicle_pos].state = RENTED;
			} else {
				ERROR_MSG_MAX_RENT_CAPACITY(c_arr[client_pos].id);
			}
		}
	}
}

void 
Vehicle_return(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len){
	int lp, vehicle_pos;

	INT_INPUT("Matricule: ", lp, NO_CONDITION);

	vehicle_pos = Vehicle_seek(v_arr, *v_arr_len, lp);

	if (vehicle_pos == -1) {
		ERROR_MSG_NOT_FOUND(lp);
	} else if (v_arr[vehicle_pos].state == RENTED) {
		v_arr[vehicle_pos].state = AVAILABLE;

		int new_mileage, id, client_pos;

		INT_INPUT("Nouveau kilometrage: ", new_mileage, new_mileage < 0);

		v_arr[vehicle_pos].mileage += new_mileage;

		INT_INPUT("CIN du client: ", id, NO_CONDITION);

		client_pos = Client_seek(c_arr, *c_arr_len, id);

		if (client_pos == -1) {
			ERROR_MSG_ATTEMPTING_TO_FIND(id);
			
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
	} else if (v_arr[vehicle_pos].state == AVAILABLE) {
		ERROR_MSG_ALREADY_AVAILABLE(lp);
	}
}

void
Vehicle_show(const Vehicle v){
	printf("Modele: %s\nMatricule: %d\nKilometrage: %d\nEtat: %s",
			v.model, v.lp,
			v.mileage, 
			(v.state == RENTED) ? "Loué" : "Disponible");
}

void 
Vehicle_show_state(const Vehicle *v_arr, int v_arr_len){
	int lp, vehicle_pos;
	INT_INPUT("Matricule: ", lp, NO_CONDITION);
	vehicle_pos = Vehicle_seek(v_arr, v_arr_len, lp);
	if (vehicle_pos == -1) {
		ERROR_MSG_NOT_FOUND(lp);
	} else {
		Vehicle_show(v_arr[vehicle_pos]);
	}
}

void 
Vehicle_show_parking_lot_state(const Vehicle *v_arr, int v_arr_len){
	printf("Nombre total des voitures: %d\n", v_arr_len);

	int rented_count = 0, available_count = 0;
	double average_mileage = 0;

	for (int i = 0; i < v_arr_len; ++i) {
		rented_count += v_arr[i].state == RENTED;
		available_count += v_arr[i].state == AVAILABLE;
		average_mileage += v_arr[i].mileage;
	}
	average_mileage = average_mileage / v_arr_len;

	printf("Nombre total des voitures louées: %d\n", rented_count);
	puts("Matricules des voitures louées: ");
	for (int i = 0; i < v_arr_len; ++i) 
		if (v_arr[i].state == RENTED)
			printf("%d\n", v_arr[i].lp);

	printf("Nombre total des voitures disponibles: %d\n", available_count);
	puts("Matricules des voitures disponibles: ");
	for (int i = 0; i < v_arr_len; ++i) 
		if (v_arr[i].state == AVAILABLE)
			printf("%d\n", v_arr[i].lp);
	printf("Kilometrage moyenne: %lf", average_mileage);
}

VehicleNode *
Vehicle_LL(Vehicle *v_arr, int v_arr_len, int type){
	VehicleNode *head = NODE_ALLOC, *next, *tmp;
	tmp = next = head;

	for (int i = 0; i < v_arr_len; ++i) {
		if (v_arr[i].state == type) {
			next->vehicle = v_arr[i];
			next->next = NODE_ALLOC;
			next = next->next;
		}
	}

	while (tmp && tmp->next != next && (tmp = tmp->next));

	if (tmp)
		tmp->next = NULL;
	
	return head;
}

void
Vehicle_LL_show(const VehicleNode *head){
	while (head) {
		Vehicle_show(head->vehicle);
		head = head->next;
	}
}
