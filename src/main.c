#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clear_buffer() {
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {}
}

void clear_console() {
    printf("\033[H\033[J");
    fflush(stdout);
}

void play_introduction() {
	clear_console();

	printf("Where am I?");
	getchar();
	
	printf("I need to get out of here!");
	getchar();
}

void play_conclusion() {
	clear_console();

	printf("I'm finally free!");
	getchar();
}

#define ROOM_ONE    0
#define ROOM_TWO    1 
#define ROOM_THREE  2 
#define ROOM_FOUR   3 
#define ROOM_FIVE   4
#define ROOM_SIX    5 
#define ROOM_SEVEN  6 
#define ROOM_EIGHT  7 
#define ROOM_NINE   8 

#define FINAL_ROOM  9 

#define MAX_ROOMS 9
#define MAX_DIALOGUES_PER_ROOM 5
#define MAX_ADJOINING_ROOMS_PER_ROOM 4
#define MAX_OBSTACLES_PER_ROOM 2

typedef struct {
	char item[20];
	char obstacle[MAX_OBSTACLES_PER_ROOM][20];
	char dialogues[MAX_DIALOGUES_PER_ROOM][100];
	short dialoguesFlags[MAX_DIALOGUES_PER_ROOM];
	short adjoiningRooms[MAX_ADJOINING_ROOMS_PER_ROOM];
} Room;

void init_rooms_data(Room *roomData) {
	Room roomOne = {
		.item = { "" },
		.obstacle = { "" },
		.dialogues = { 
			"This room is empty...", 
			"I see a door in the right direction..." 
		},
		.dialoguesFlags = { 1, 1, 0, 0, 0 },
		.adjoiningRooms = { 0, 0, ROOM_TWO, 0 }
	};


	Room roomTwo = {
		.item = { "cheese" },
		.obstacle = { "" },
		.dialogues = {
			"I see a piece of cheese...",
			"I see a door in the down and left direction...",
			"There's nothing else in this room..."
		},
		.dialoguesFlags = { 1, 1, 1, 0, 0 },
		.adjoiningRooms = { 0, ROOM_FIVE, 0, ROOM_ONE }
	};

	Room roomThree = {
		.item = { "key" },
		.obstacle = { "" },
		.dialogues = {
			"I see a key, it has to be the key for the exit door!",
			"I see a door in the down direction...",
			"There's nothing else in this room..."
		},
		.dialoguesFlags = { 1, 1, 1, 0, 0 },
		.adjoiningRooms = { 0, ROOM_SIX, 0, 0 }
	};

	Room roomFour = {
		.item = { "hammer" },
		.obstacle = { "" },
		.dialogues = {
			"I see a hammer, maybe I can use it to break something...",
			"I see a door in the down direction...",
			"There's nothing else in this room..."
		},
		.dialoguesFlags = { 1, 1, 1, 0, 0 },
		.adjoiningRooms = { 0, ROOM_SEVEN, 0, 0 }
	};

	Room roomFive = {
		.item = { "" },
		.obstacle = { "cheese", "hammer" },
		.dialogues = {
			"I see a rat blocking the down door, I need to distract it with something...",
			"The handle of the right door seems stuck...",
			"I see a door in the up, right and down direction..."
		},
		.dialoguesFlags = { 1, 1, 1, 0, 0 },
		.adjoiningRooms = { ROOM_TWO, 0, 0, 0 }
	};

	Room roomSix = {
		.item = { "" },
		.obstacle = { "key" },
		.dialogues = {
			"The door in the down direction is blocked, I have to find a key!",
			"I see a door in the up, left and down direction..."
		},
		.dialoguesFlags = { 1, 1, 0, 0, 0 },
		.adjoiningRooms = { ROOM_THREE, 0, 0, ROOM_FIVE }
	};

	Room roomSeven = {
		.item = { "" },
		.obstacle = { "" },
		.dialogues = {
			"This room it's empty...",
			"I see a door in the up and right direction..."
		},
		.dialoguesFlags = { 1, 1, 0, 0, 0 },
		.adjoiningRooms = { ROOM_FOUR, 0, ROOM_EIGHT, 0 }
	};

	Room roomEight = {
		.item = { "" },
		.obstacle = { "" },
		.dialogues = {
			"I see a door in the up and left direction...",
			"There's nothing else in this room..."
		},
		.dialoguesFlags = { 1, 1, 0, 0, 0 },
		.adjoiningRooms = { ROOM_FIVE, 0, 0, ROOM_SEVEN }
	};

	Room roomNine = {
		.item = { "" },
		.obstacle = { "" },
		.dialogues = {
			"I see the exit door in the right direction!",
			"Finally, I can escape this house!"
		},
		.dialoguesFlags = { 1, 1, 0, 0, 0 },
		.adjoiningRooms = { ROOM_SIX, 0, FINAL_ROOM, 0 }
	};

	roomData[ROOM_ONE] = roomOne;
	roomData[ROOM_TWO] = roomTwo;
	roomData[ROOM_THREE] = roomThree;
	roomData[ROOM_FOUR] = roomFour;
	roomData[ROOM_FIVE] = roomFive;
	roomData[ROOM_SIX] = roomSix;
	roomData[ROOM_SEVEN] = roomSeven;
	roomData[ROOM_EIGHT] = roomEight;
	roomData[ROOM_NINE] = roomNine;
}

void set_room(short *houseRooms, short newRoom) {
    *houseRooms = newRoom;
}

void print_room_dialogue(Room *roomsData, short currentRoom) {
	short dialogueFlagsLenght = sizeof(roomsData[currentRoom].dialoguesFlags) / sizeof(roomsData[currentRoom].dialoguesFlags[0]);

	for (int i = 0; i < dialogueFlagsLenght; i++) {
		if (roomsData[currentRoom].dialoguesFlags[i] != 1) {
			continue;
		}	

		printf("%s", roomsData[currentRoom].dialogues[i]);
		getchar();
	}
}

	short try_move_to(Room *roomsData, short *currentRoom, char direction[]) {
		const char *directions[4] = { "up", "down", "right", "left" };

		short nextRoom = -1;

		for (int i = 0; i < MAX_ADJOINING_ROOMS_PER_ROOM; i++) {
			if (roomsData[*currentRoom].adjoiningRooms[i] == 0) {
				continue;
			}

			if (strcmp(directions[i], direction) != 0) {
				continue;
			}

			nextRoom = roomsData[*currentRoom].adjoiningRooms[i];
			break;
		}

		if (nextRoom != -1) {
			set_room(*&currentRoom, nextRoom);
			return 1;
		}

		return 0;
	}

short try_take_item(Room *roomsData, short *currentRoom, char item[]) {
	if (strcmp(roomsData[*currentRoom].item, item) == 0) {
		roomsData[*currentRoom].dialoguesFlags[0] = 0;

		clear_console();

		printf("I took the %s item.\n", item);
		getchar();

		return 1;
	}

	return 0;
}

short try_use_item(Room *roomsData, short *currentRoom, char item[]) {
	short lenght = sizeof(roomsData[*currentRoom].obstacle) / sizeof(roomsData[*currentRoom].obstacle[0]);

	for (int i = 0; i < lenght; i++) {
		if (strcmp(roomsData[*currentRoom].obstacle[i], item) != 0) {
			continue;
		}

		if (strcmp(roomsData[*currentRoom].obstacle[i], "cheese") == 0) {
			roomsData[*currentRoom].dialoguesFlags[0] = 0;
			roomsData[*currentRoom].adjoiningRooms[1] = ROOM_EIGHT; 
		}
		else if (strcmp(roomsData[*currentRoom].obstacle[i], "hammer") == 0) {
			roomsData[*currentRoom].dialoguesFlags[1] = 0;
			roomsData[*currentRoom].adjoiningRooms[2] = ROOM_SIX;
		}
		else if (strcmp(roomsData[*currentRoom].obstacle[i], "key") == 0) {
			roomsData[*currentRoom].dialoguesFlags[0] = 0;
			roomsData[*currentRoom].adjoiningRooms[1] = ROOM_NINE;
		}

		return 1;
	}

	return 0;
}

int main() {
	play_introduction();

	short houseRooms;

	set_room(&houseRooms, ROOM_ONE);
	
	Room roomsData[MAX_ROOMS];

	init_rooms_data(roomsData);

	while (houseRooms != FINAL_ROOM) {
		clear_console();

		print_room_dialogue(roomsData, houseRooms);

		char playerAction[20];

		printf("\nEnter your action:\n");

		scanf("%s", playerAction);

		clear_buffer();

		if (try_move_to(roomsData, &houseRooms, playerAction)) {}
		else if (try_take_item(roomsData, &houseRooms, playerAction)) {}
		else if (try_use_item(roomsData, &houseRooms, playerAction)) {}
		else {
			clear_console();
			printf("That action, it's not defined in this context.\n");
			getchar();
		}
	}

	play_conclusion();
	return 0;
}