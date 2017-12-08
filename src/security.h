#define ROOT        0
#define ADMIN       1
#define BASIC       2
#define RESTRICTED -1

#define USER_LIMIT  20

typedef struct User_t {
    int authority;
    char *username;
    char *password;
} User;

void LoadUsers();
void SaveUsers();
int UserLogin();
User GetUser();
int ValidateCredentials(char* username, char* password);
int CreateUser(char* username, char* password, int authority, int authToken);
int RemoveUser(char* username, int authToken);
int ChangePassword(char* username, char* username_exe);
int AddAdmin(char* username, int authToken);
int RemoveAdmin(char* username, int authToken);
