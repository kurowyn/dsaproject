#ifndef UTILS_H
#define UTILS_H

#define MAX_BUFFER_LEN 1024
#define NO_CONDITION 0

#ifdef WIN32
#define CLS system("cls")
#else
#define CLS system("clear")
#endif

// These functions make input a lot less of a pain in the back.
// Of course, they may not be wholly safe, but this is just some college project.
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

typedef enum State {RENTED = 1, AVAILABLE = 0} State;

typedef enum Option {OPTION_EXIT = 0,
					 OPTION_RENT_VEHICLE = 1,
					 OPTION_RETURN_VEHICLE = 2,
					 OPTION_SHOW_VEHICLE_STATE = 3,
					 OPTION_SHOW_PARKING_LOT_STATE = 4,
					 OPTION_SHOW_AND_STORE_AVAILABLE_VEHICLES = 5,
					 OPTION_SHOW_AND_STORE_RENTED_VEHICLES = 6,} Option;

typedef enum MacroOption {NORMAL_SERACH, USE_PARAM_SEARCH} MacroOption;

typedef struct Client Client;

typedef struct Vehicle Vehicle;

typedef struct GenericArr GenericArr; 

struct Vehicle {
	char model[MAX_BUFFER_LEN]; 	
	int lp, mileage;
	State state;
};

struct Client {
	char last_name[MAX_BUFFER_LEN], first_name[MAX_BUFFER_LEN];
	int id, telephone, *vehicle_lp_arr, len;
};

int Client_seek(Client *c_arr, int len, int id);

int Vehicle_seek(Vehicle *v_arr, int len, int lp);

int Client_vehicle_seek(const Client c, int lp);

void Client_delete_vehicle(Client *c, int vehicle_pos);

void Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len);

Vehicle Vehicle_init(char *model, int lp, int mileage, State state);

Client Client_init(char *first_name, char *last_name, int id, int telephone, 
				   int *vehicle_lp_arr, int len);

Client Client_init_interactive(void);

Vehicle Vehicle_init_interactive(void);

void Vehicle_return(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len);

void Vehicle_show_state(const Vehicle *v_arr, int v_arr_len);

void Vehicle_show_parking_lot_state(const Vehicle *v_arr, int v_arr_len);

void Available_vehicle_LL();

void Rented_vehicle_LL();

void _pause(void);

void _Client_arr_show(Client *c_arr, int c_arr_len);


void _Vehicle_rent(Vehicle *v_arr, Client *c_arr, int *v_arr_len, int *c_arr_len,
				   int lp, Client c);

// Define the LL struct later.

#endif // UTILS_H
