#ifndef UTILS_H
#define UTILS_H

// Some remarks:
// lp: license plate
// c_arr: client array, v_arr: vehicle array
// len: length
// pos: position
// All other variables also follow the same conventions.

#define MAX_BUFFER_LEN 1024
#define MAX_ARR_LEN 50
#define NO_CONDITION 0
#define NODE_ALLOC malloc(sizeof (VehicleNode))

#define ERROR_MSG_NOT_FOUND(lp) \
{ \
	printf("ERREUR: %d, aucune telle voiture.", lp); \
} \

#define ERROR_MSG_ALREADY_RENTED(lp) \
{ \
	printf("ERREUR: %d, déjà loué.", lp); \
} \

#define ERROR_MSG_ALREADY_AVAILABLE(lp) \
{ \
	printf("ERREUR: %d, déjà disponible.", lp); \
} \

#define ERROR_MSG_MAX_RENT_CAPACITY(id)	 \
{ \
	printf("ERREUR: Le client %d a déjà atteint le nombre maximal des voitures (%d)", \
		   id, MAX_ARR_LEN);  \
} \

#define ERROR_MSG_MAX_CLIENT_CAPACITY  \
{ \
	printf("ERREUR: Nombre maximal des clients atteint.", MAX_ARR_LEN);  \
} \

#define ERROR_MSG_ATTEMPTING_TO_FIND(id)  \
{ \
	printf("ERREUR: échec du trouver un tel client (%d). Le system va essayer" \
		   "de le retrouver.", id); \
} \

#define MAIN_MENU  \
{ \
	puts("1: Location d'une voiture"); \
	puts("2: Retour d'une voiture"); \
	puts("3: Etat d'une voiture"); \
	puts("4: Etat d'un parc des voitures"); \
	puts("5: Stockage et affichage des voitures disponibles"); \
	puts("6: Stockage et affichage des voitures en location"); \
	puts("0: Fin du programme"); \
} \

#ifdef WIN32
	#define CLS system("cls")
#else
	#define CLS system("clear")
#endif

// These functions make input a lot less of a pain in the back.
// Of course, they may not be wholly safe, but they are enough for our
// purposes. They may also use a little too much stack memory, also a negligeable
// concern for our purposes.
// How these functions work is self explanatory, I hope.
#define STRING_INPUT(prompt, string, continue_condition) \
{ \
	do { \
		printf("%s", prompt); \
		fgets(string, MAX_BUFFER_LEN, stdin); \
	} while ((continue_condition)); \
	if (string[strlen(string)] == '\n' && (string[strlen(string)] = '\0')); \
} \

#define INT_INPUT(prompt, int_var, continue_condition) \
{ \
	char buffer[MAX_BUFFER_LEN], *endptr; \
	do { \
		printf("%s", prompt); \
		fgets(buffer, MAX_BUFFER_LEN, stdin); \
		if (buffer[strlen(buffer) - 1] == '\n' \
			&& (buffer[strlen(buffer) - 1] = '\0')); \
		int_var = strtoll(buffer, &endptr, 10); \
	} while ((continue_condition) || *endptr != '\0'); \
} \

#define TYPEDEF_STRUCT(struct_name) typedef struct struct_name struct_name

typedef enum State {RENTED = 1, AVAILABLE = 0} State;

// Looks much nicer than rows of defines.
typedef enum Option {OPTION_EXIT = 0,
					 OPTION_RENT_VEHICLE = 1,
					 OPTION_RETURN_VEHICLE = 2,
					 OPTION_SHOW_VEHICLE_STATE = 3,
					 OPTION_SHOW_PARKING_LOT_STATE = 4,
					 OPTION_SHOW_AND_STORE_AVAILABLE_VEHICLES = 5,
					 OPTION_SHOW_AND_STORE_RENTED_VEHICLES = 6,} Option;

TYPEDEF_STRUCT(Client);
TYPEDEF_STRUCT(Vehicle);
TYPEDEF_STRUCT(VehicleNode);

struct Vehicle {
	char model[MAX_BUFFER_LEN]; 	
	int lp, mileage;
	State state;
};

struct VehicleNode {
	Vehicle vehicle;
	VehicleNode *next;
};

struct Client {
	char last_name[MAX_BUFFER_LEN], first_name[MAX_BUFFER_LEN];
	int id, telephone, lp_arr[MAX_ARR_LEN], lp_arr_len;
};

int Client_seek(Client *c_arr, int c_arr_len, int id);
int Vehicle_seek(Vehicle *v_arr, int v_arr_len, int lp);
int Client_vehicle_seek(const Client c, int lp);
void Client_delete_vehicle(Client *c, int vehicle_pos);
void Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len);
Client Client_init_interactive(void);
Vehicle Vehicle_init_interactive(void);
void Vehicle_return(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len);
void Vehicle_show_state(const Vehicle *v_arr, int v_arr_len);
void Vehicle_show_parking_lot_state(const Vehicle *v_arr, int v_arr_len);
void _pause(void);
VehicleNode *Vehicle_LL(Vehicle *v_arr, int v_arr_len, int type);
void Vehicle_LL_show(const VehicleNode *head);
void Vehicle_show(const Vehicle v);

#endif // UTILS_H
