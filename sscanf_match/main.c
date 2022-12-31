#include <stdio.h>

int main()
{
	const char* str = "1062235 '172.17.1.191-cc:a2' '/Default' 172.17.1.191 32:43:e4:5b:cc:a2 '2022-12-06 11:24:26' 1 1048575";
	printf("str: %s\n", str);

	char id[128];
	char objectName[128];
	char objectGroupPath[128];
	char ip[128];
	char mac[128];
	char accessTime[128];
	char action[128];
	char details[128];

	sscanf(str, "%s %s %s %s %s %21[-,:,', ,0-9] %s %s", id, objectName, objectGroupPath, ip, mac, accessTime, action, details);

	printf("id: %s\n", id);
	printf("objectName: %s\n", objectName);
	printf("objectGroupPath: %s\n", objectGroupPath);
	printf("ip: %s\n", ip);
	printf("mac: %s\n", mac);
	printf("accessTime: %s\n", accessTime);
	printf("action: %s\n", action);
	printf("details: %s\n", details);
	
	return 0;
}
