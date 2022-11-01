#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#define true 1
#define false 0

int main(){
	int inFromFile = true;
	char* usersPath = "users.txt";
	char* passwordsPath = "passwords.txt";
	FILE *userf = fopen(usersPath, "r");
	FILE *passwordsf = fopen(passwordsPath, "r");


	FILE *passf;
	passf = fopen("passwd", "w");
	FILE *shadowf = fopen("shadow", "w");
	//this uses the real passwd file to get current user.
	unsigned int userId;
	char* username;
	//char* homeDir = "/users/guest/s/sammyb98";
	char* homeDir = "/";
       	//char* shell = "/bin/bash";
	char* shell = "/usr/sbin/nologin";
	int groupId = 1;
	char* password;
	char* hashedSaltedPass;
	//1 is the id for MD5
	char* salt = "$1$MwqlDhGI";
	if(inFromFile){
		size_t len = 0;
    		ssize_t read;
		userId = 6;
		while((read = getline(&username, &len, userf)) > -1){
			username[read-1] = '\0';
			fprintf(passf, "%s:x:%d:%d:pokemon:%s:%s\n", username, userId, groupId, homeDir, shell);
			//adding this so we have a unique userid for each user.
			userId++;
			read = getline(&password, &len, passwordsf);
			password[read-1] = '\0';
			hashedSaltedPass = crypt(password, salt);
			fprintf(shadowf, "%s:%s:%d:%d:%d:%d:::\n", username, hashedSaltedPass, 5, 0, 99999, 0);
			//printf("Length: %ld\n", read);


		}
	}
	//TODO: Make default if not reading users from file
	else{
		userId = getuid();
        	struct passwd *p = getpwuid(userId);
		username = p->pw_name;
		for(int i = 0; i < 10; i++){
			fprintf(passf, "%s:x:%d:SamuelBennion:%s:%s\n", username, userId, homeDir, shell);
			//fprinf(shadowf, "%s:%s:%d:%d:%d:%d", username, );
		}
	}	
	//fprintf(passf, "%s:x:%d:Samuel Bennion:%s:/bin/bash\n", username, userId, homeDir);
	fclose(userf);
	fclose(passwordsf);
	fclose(passf);
	fclose(shadowf);

	return 0;
}
