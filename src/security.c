#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "security.h"


User *users;
int currentUser;
int userCount;

int findUser();
// int test();

// int main()
// {

// 	LoadUsers();
// 	UserLogin();
// 	return 0;
// }

void LoadUsers()
{
	/*
	 *	Format to load:
	 *
	 *  userinfo.txt:
	 *	<userCount>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	...
	 *
	 */
	FILE *fp = fopen("userinfo.txt", "r");
	fscanf(fp, "%i", &userCount);

	users = malloc(sizeof(User) * USER_LIMIT);

	int unLength, pwLength;
	for (int i = 0; i < userCount; i++) {
		fscanf(fp, "%i", &unLength) ;
		fscanf(fp, "%i", &pwLength);
		fscanf(fp, "%i", &users[i].authority);
		char *username = malloc(sizeof(char) * (unLength + 1));
		char *password = malloc(sizeof(char) * (pwLength + 1));
		username[unLength] = '\0';
		password[pwLength] = '\0';

		fscanf(fp, "%s", username);
		fscanf(fp, "%s", password);
		users[i].username = username;
		users[i].password = password;
	}
	fclose(fp);
}

void SaveUsers()
{
	/*
	 *	Format to load:
	 *
	 *  userinfo.txt:
	 *	<userCount>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	<unLength> <pwLength> <authority> <username> <password>
	 *	...
	 *
	 */
	FILE *fp = fopen("userinfo.txt", "w");
	fprintf(fp, "%i\n", userCount);
	for (int i = 0; i < userCount; i++) {
		fprintf(fp, "%i ", (int)strlen(users[i].username));
		fprintf(fp, "%i ", (int)strlen(users[i].password));
		fprintf(fp, "%i ", users[i].authority);
		fprintf(fp, "%s ", users[i].username);
		fprintf(fp, "%s\n", users[i].password);
	}
	fclose(fp);
}

int UserLogin() 
{
	char username[16];
	char password[16];
	printf("\nPlease enter your username and password.");
	
	printf("\nUsername: ");
	fgets(username, sizeof(char*) * 16, stdin);
	username[strlen(username) - 1] = '\0';
	printf("\nPassword: ");
	fgets(password, sizeof(char*) * 16, stdin);
	password[strlen(password) - 1] = '\0';
			
	if(ValidateCredentials(username, password)) {
		char answer[8];
		printf("\nIt seems you don't have an account or have forgotten your password");
		printf("\nWould you like to create a new account? (y/n): ");
		fgets(answer, sizeof(char*) *8 , stdin);
		printf("\n");
		if (answer[0] == 'y') {
			if(findUser(username) == -1) {
				CreateUser(username, password, BASIC, ROOT);
			}
			else {
				printf("\nPlease try again with a different name.");
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	return findUser(username);
}

User GetUser(int id){
	return users[id];
}

int ValidateCredentials(char* username, char* password)
{
	int id = findUser(username);
	if (id == -1) {
		printf("\nERROR: No user by this username.");
		return -1;
	}
	if(!strcmp(users[id].username, username) && !strcmp(users[id].password, password)) {
		printf("\nLogged in!");
		return 0;
	}
	return -1;
}

/*
 *  Only root administrator and administrators are allowed to create users.
 */
int CreateUser(char* username, char* password, int authority, int authToken)
{
	if (authToken != ROOT && authToken != ADMIN){
		printf("\nERROR: You must have administrative privledges to create users.");
		return -1;
	}
	if(userCount == USER_LIMIT) {
		printf("\nERROR: No more users can be created.");
		return -1;
	}
	if(findUser(username) != -1){
		printf("\nERROR: User exists.");
		return -1;
	}
	users[userCount].username = username;
	users[userCount].password = password;
	users[userCount].authority = authority;
	userCount++;
	printf("\nUser Created.");
	SaveUsers();
	LoadUsers();
	return 0;
}

/*
 * 	Only root admin and other admins are allowed to remove users.
 *	Only the root administrator is allowed to remove an administrator.
 *	Admin cannot remove its own account.
 *	Root administrator connot be removed at all.
 *  Checks that the user exists before trying to remove it.
 *	If successfull, it should state that the user has been removed.
 */
int RemoveUser(char* username, int authToken)
{
	if (authToken != ROOT && authToken != ADMIN) {
		printf("\nERROR: You must have administrative privledges to remove users.");
		return -1;
	}
	
	int id = findUser(username);
	
	if (id == -1) {
		printf("\nERROR: User does not exist.");
		return -1;
	}
	if (users[id].authority == ROOT) {
		printf("\nYou can't remove ROOT, ROOT removes you.");
		return -1;
	}
	if (users[id].authority == ADMIN && authToken != ROOT) {
		printf("\nERROR: You must be ROOT to remove an ADMIN.");
		return -1;
	}
	FILE *fp = fopen("userinfo.txt", "w");
	userCount--;
	fprintf(fp, "%i\n", userCount);
	for (int i = 0; i < userCount + 1; i++) {
		if (i != id){
			fprintf(fp, "%i ", (int)strlen(users[i].username));
			fprintf(fp, "%i ", (int)strlen(users[i].password));
			fprintf(fp, "%i ", users[i].authority);
			fprintf(fp, "%s ", users[i].username);
			fprintf(fp, "%s\n", users[i].password);
			//printf("\nPrinting %s...\n", users[i].username);
		}
	}
	fclose(fp);
	printf("\nUser Removed.");
	LoadUsers();
	return 0;
}

/*
 * Any user is allowed to change its own password.
 * The root admin and the admins are allowed to change basic user passwords.
 * Admins can't change another admin's password. ROOT can change admin passwords
 */
int ChangePassword(char* username, char* username_exe)
{
	int id = findUser(username);
	int id_exe = findUser(username_exe);
	char newPass[16] = "";
	char newPass_confirm[16] = "";

	//printf("\nExecuting id:%i\tResulting id:%i\n", id_exe, id);

	if(id == -1){
		printf("\nERROR: User does not exist.");
		return -1;
	}

	if (id_exe != id){
		if (users[id_exe].authority >= users[id].authority) {
			printf("\nYou can't change another user password without admin privledges.");
			return -1;
		}
	}

	printf("\nPlase enter a new password: ");
	fgets(newPass, 16, stdin);
	printf("\nPlease enter the new password again to confirm: ");
	fgets(newPass_confirm, 16, stdin);
	if (strcmp(newPass, newPass_confirm)) {
		printf("\nPasswords to not match. Password not changed.");
		return -1;
	}
	newPass[strlen(newPass) - 1] = '\0';
	users[id].password = newPass;
	SaveUsers();
	LoadUsers();
	printf("\nPassword changed.");
	return 0;
}

/*
 *	Gives a user admin privledges
 */
int AddAdmin(char* username, int authToken)
{
	int id = findUser(username);
	if (id == -1){
		printf("\nERROR: User does not exist.");
		return -1;
	}
	if (authToken == BASIC){
		printf("\nERROR: You must have administrative privleges to do that.");
		return -1;
	}
	if (users[id].authority == ROOT) {
		printf("\nERROR: ROOT is ROOT");			
		return -1;
	} 
	if (users[id].authority == ADMIN) {
		printf("\nERROR: Cannot be higher than Admin");
		return -1;
	}
	users[id].authority = ADMIN;
	SaveUsers();
	printf("\nAdmin added.");
	return 0;
}

/*
 * Removes administrator privleges from  auser.
 * Check that the user exists in the system and that the user is an administrator
 */
int RemoveAdmin(char* username, int authToken) 
{
	int id = findUser(username);
	if (id == -1) {
		printf("\nERROR: User does not exist.");
		return -1;
	}
	if (authToken == BASIC) {
		printf("\nERROR: You must have administrative privleges to do that");
		return -1;
	}
	if (users[id].authority == BASIC) {
		printf("\nERROR: This user is not an admin.");
		return -1;
	}
	if (authToken != ROOT) {
		printf("\nERROR: Only root admins can remove admins.");
		return -1;
	}

	users[id].authority = BASIC;
	SaveUsers();
	printf("\nThe user %s no longer has admin privleges\n", username);
	return 0;
}

int findUser(char* username)
{
	for(int i = 0; i < userCount; i++){
		if (!strcmp(users[i].username, username)){
			//printf("\nUser Found");
			return i;
		}
	}
	//printf("\nUser Not Found");
	return -1;
}