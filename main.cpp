#include<stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mysql.h>
#include <mysqld_error.h> 

  
struct ttsv{
	char hoTen[50];
	int mssv;
	float dV, dT, dA;
};

struct ttsv dssv[100];
int slsv = 0;

MYSQL *conn;

void connectToDB() {
    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "root", "123456789", "qlsv", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
}

void insertToDB(struct ttsv sv) {
    char query[256];
    sprintf(query, "INSERT INTO SinhVien (hoTen, mssv, dV, dT, dA) VALUES ('%s', %d, %.2f, %.2f, %.2f)",
            sv.hoTen, sv.mssv, sv.dV, sv.dT, sv.dA);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
}

void updateInDB(struct ttsv sv) {
    char query[256];
    sprintf(query, "UPDATE SinhVien SET hoTen='%s', dV=%.2f, dT=%.2f, dA=%.2f WHERE mssv=%d",
            sv.hoTen, sv.dV, sv.dT, sv.dA, sv.mssv);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
}

void deleteFromDB(int mssv) {
    char query[128];
    sprintf(query, "DELETE FROM SinhVien WHERE mssv=%d", mssv);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
}

void deleteAllFromDB() {
    char query[128] = "DELETE FROM SinhVien";

    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
}

void fetchStudents() {
    char query[1000];
    sprintf(query, "SELECT hoTen, mssv, dV, dT, dA FROM SinhVien");
	
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }

    int num_rows = mysql_num_rows(result);
    
    slsv = num_rows;  

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        strcpy(dssv[i].hoTen, row[0] ? row[0] : "NULL");
        dssv[i].mssv = atoi(row[1] ? row[1] : "0");
        dssv[i].dV = atof(row[2] ? row[2] : "0");
        dssv[i].dT = atof(row[3] ? row[3] : "0");
        dssv[i].dA = atof(row[4] ? row[4] : "0");
        i++;
    }

    mysql_free_result(result);
}

void closeDB() {
    mysql_close(conn);
}

int checkMSSV(int mssv) {
    for (int i = 0; i < slsv; i++) {
        if (mssv == dssv[i].mssv) {
            return 1; 
        }
    }
    return 0; 
}

int removeSpaces(char *str){
	int length = strlen(str);
	
	while(*str == ' '){
		for(int i = 0; i < length; i++){
			str[i] = str[i + 1];
		}
		length--;
	}
	
	for(int i = 0; i < length; i++){
		while(str[i] == ' ' && str[i + 1] == ' '){
			for(int j = i; j < length; j++){
				str[j] = str[j + 1];
			}
			length--;
		}
	}
	
	while(str[length - 1] == ' '){
		str[length - 1] = '\0';
		length--;
	}
}

int isAllAlpha(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != ' ') {
            return 0;
        }
    }
    return 1;
}

void addSV(){

	printf("Nhap thong tin cho sinh vien moi:\n");
	bool isInvalidInput;
	
	do {
	    printf("Ho ten: ");
	    getchar();
	    scanf("%[^\n]", dssv[slsv].hoTen);
	
		removeSpaces(dssv[slsv].hoTen);
	
	    isInvalidInput = (strlen(dssv[slsv].hoTen) == 0 || !isAllAlpha(dssv[slsv].hoTen));
	
	    if (isInvalidInput) {
        	printf("Vui long nhap ho ten gom cac chu cai.\n");
	    }
	
	} while (isInvalidInput);
	    
    int mssv;
    do {
        printf("Ma so sinh vien: ");
        while (scanf("%d", &mssv) != 1) {
            printf("Vui long nhap lai ma so sinh vien la so nguyen.\n");
            while (getchar() != '\n');
        }
        if (checkMSSV(mssv)) {
            printf("Ma so sinh vien da ton tai. Vui long nhap lai.\n");
        } else {
            dssv[slsv].mssv = mssv;
        }
    } while (checkMSSV(mssv));
    
    printf("Diem Van: ");
    while (scanf("%f", &dssv[slsv].dV) != 1 || dssv[slsv].dV < 0 || dssv[slsv].dV > 10) {
        printf("Vui long nhap lai diem Van trong khoang tu 0 den 10.\n");
        while (getchar() != '\n'); 
        printf("Diem Van: ");
    }
    printf("Diem Toan: ");
    while (scanf("%f", &dssv[slsv].dT) != 1 || dssv[slsv].dT < 0 || dssv[slsv].dT > 10) {
        printf("Vui long nhap lai diem Toan trong khoang tu 0 den 10.\n");
        while (getchar() != '\n'); 
        printf("Diem Toan: ");
    }
    printf("Diem Anh: ");
    while (scanf("%f", &dssv[slsv].dA) != 1 || dssv[slsv].dA < 0 || dssv[slsv].dA > 10) {
        printf("Vui long nhap lai diem Anh trong khoang tu 0 den 10.\n");
        while (getchar() != '\n'); 
        printf("Diem Anh: ");
    }
    insertToDB(dssv[slsv]);
    slsv++;
    printf("Them sinh vien thanh cong.\n"); 
}

void showTTSV(int i){
	printf("|\t%-20s|\t%-20d|\t%-15.2f|\t%-15.2f|\t%-15.2f|\n", dssv[i].hoTen, dssv[i].mssv, dssv[i].dV, dssv[i].dT, dssv[i].dA);
} 

void showDSSV(){
	printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
	printf("|\t%-20s|\t%-20s|\t%-15s|\t%-15s|\t%-15s|\n", "Ho Va Ten", "Ma So Sinh Vien", "Diem Van", "Diem Toan", "Diem Anh"); 
	printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
	for(int i = 0; i < slsv; i++){
		showTTSV(i); 
	}
	printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
}

void search(){
	int ktM;
	printf("Nhap ma sinh vien: "); 
	scanf("%d", &ktM);
	getchar();
	int found=0; 
	for(int i = 0; i < slsv; i++){
		if(ktM == dssv[i].mssv){
			found = 1;
			printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
			printf("|\t%-20s|\t%-20s|\t%-15s|\t%-15s|\t%-15s|\n", "Ho Va Ten", "Ma So Sinh Vien", "Diem Van", "Diem Toan", "Diem Anh"); 
			printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
			showTTSV(i); 
			printf("+---------------------------+-----------------------+------------------+-----------------------+-----------------------+\n"); 
		} 
	} 
	if(found==0) {
		printf("Khong tim thay ma sinh vien.\n"); 
	}
} 

void edit(){
    int found = 0;
    int ktM;
    printf("Nhap ma so sinh vien can sua: ");
    scanf("%d", &ktM);
    getchar();
    for(int i = 0; i < slsv; i++){
        if(ktM == dssv[i].mssv){
            found = 1;
			printf("Ho ten (truoc khi sua: %s): ", dssv[i].hoTen);
            do {
                
                scanf(" %[^\n]s", dssv[i].hoTen); 

                removeSpaces(dssv[i].hoTen);

                if (!isAllAlpha(dssv[i].hoTen)){
                    printf("Ten khong hop le. Vui long nhap lai.\n");
                }
            } while (!isAllAlpha(dssv[i].hoTen));

            printf("Diem Van (truoc khi sua: %.2f): ", dssv[i].dV);
		    while (scanf("%f", &dssv[i].dV) != 1 || dssv[i].dV < 0 || dssv[i].dV > 10) {
		        printf("Vui long nhap lai diem Van trong khoang tu 0 den 10.\n");
		        while (getchar() != '\n'); 
		        printf("Diem Van: ");
		    }            
			getchar();

            printf("Diem Toan (truoc khi sua: %.2f): ", dssv[i].dT);
		    while (scanf("%f", &dssv[i].dT) != 1 || dssv[i].dT < 0 || dssv[i].dT > 10) {
		        printf("Vui long nhap lai diem Toan trong khoang tu 0 den 10.\n");
		        while (getchar() != '\n'); 
		        printf("Diem Toan: ");
		    }              
            getchar();

            printf("Diem Anh (truoc khi sua: %.2f): ", dssv[i].dA);
		    while (scanf("%f", &dssv[i].dA) != 1 || dssv[i].dA < 0 || dssv[i].dA > 10) {
		        printf("Vui long nhap lai diem Anh trong khoang tu 0 den 10.\n");
		        while (getchar() != '\n'); 
		        printf("Diem Anh: ");
		    }              
            getchar();

            updateInDB(dssv[i]);
			printf("Sua sinh vien thanh cong.\n"); 
            break; 
        }
    }
    if(found == 0){
        printf("Khong tim thay ma sinh vien.\n"); 
    }
}

void deleted(){
	int ktM;
	int found=0;
	printf("Nhap ma sinh vien: "); 
	scanf("%d", &ktM);
	getchar(); 
	for(int i = 0; i < slsv; i++){
		if(ktM == dssv[i].mssv){
			found = 1; 
			for(int j = i; j < slsv-1;j++){
				dssv[j] = dssv[j+1]; 
			}
			--slsv;
			deleteFromDB(ktM);
			printf("Xoa sinh vien thanh cong.\n");			
		} 
	}
	if(found == 0){
		printf("Khong tim thay ma sinh vien.\n"); 
	} 
}

void deleteAll(){
    for(int i = 0; i < slsv; i++) { 
        dssv[i].mssv = 0;
        dssv[i].hoTen[0] = '\0';
        dssv[i].dV = 0.0;
        dssv[i].dT = 0.0;
        dssv[i].dA = 0.0;
    }
    slsv = 0;
    deleteAllFromDB();
    printf("Da xoa toan bo sinh vien khoi danh sach.\n");
}

void dTBSV(int i, float *tb){
	float sum = dssv[i].dV + dssv[i].dT + dssv[i].dA;
	*tb = sum/3;
	 
} 

void dTBDSSV(){
	printf("+---------------------------+-----------------------+-----------------------+\n"); 
	printf("|\t%-20s|\t%-20s|\t%-20s|\n", "Ho Va Ten", "Ma So Sinh Vien", "Diem Trung Binh");
	printf("+---------------------------+-----------------------+-----------------------+\n"); 
	float tb = 0;
	float all = 0; 
	for(int i = 0; i < slsv; i++){
		dTBSV(i, &tb);
		printf("|\t%-20s|\t%-20d|\t%-20.2f|\n", dssv[i].hoTen, dssv[i].mssv, tb);
		all += tb; 
	}
	printf("+---------------------------+-----------------------+-----------------------+\n");
	printf("|\tDiem trung binh cua toan bo sinh vien:\t\t%-20.2f|\n", all/slsv);
	printf("+---------------------------+-----------------------+-----------------------+\n");
} 

void saveFile(){
	FILE *f = fopen("danhSachSV.txt", "w");
	if(f == NULL){
		printf("Loi khong the mo file.\n");
		return; 
	}
	for(int i = 0; i < slsv; i++){
		fprintf(f, "%s\n %d\n %.2f\n %.2f\n %.2f\n", dssv[i].hoTen, dssv[i].mssv, dssv[i].dV, dssv[i].dT, dssv[i].dA); 
	} 
	fclose(f);
	printf("Du lieu da duoc luu vao file."); 
} 

void readFile(){
	FILE *f = fopen("danhSachSV.txt", "r");
	if(f == NULL){
		printf("Loi khong the mo file.\n");
		return; 
	} 
	while(fscanf(f, "%[^\n]\n %d\n %f\n %f\n %f\n", dssv[slsv].hoTen, &dssv[slsv].mssv, &dssv[slsv].dV, &dssv[slsv].dT, &dssv[slsv].dA) != EOF){
		slsv++; 
	}
	fclose(f);
	printf("Du lieu da duoc doc tu file."); 
	
	for(int i = 0; i < slsv; i++) {
        insertToDB(dssv[i]);
    }

	
} 

bool isEmpty() {
    return slsv == 0;
}

void menu(){
	printf("\n\n------------------------- CHUONG TRINH QUAN LY SINH VIEN -------------------------\n\n");
	printf("\t0. Xoa toan bo danh sach sinh vien.\n");
	printf("\t1. Them thong tin cua mot sinh vien moi.\n");
	printf("\t2. Hien thi thong tin cua sinh vien.\n");
	printf("\t3. Tim kiem thong tin cua mot sinh vien.\n");
	printf("\t4. Sua doi thong tin cua mot sinh vien.\n");
	printf("\t5. Xoa thong tin cua mot sinh vien.\n");
	printf("\t6. Thong ke diem trung binh cua sinh vien.\n");
	printf("\t7. Luu danh sach sinh vien vao file.\n");
	printf("\t8. Lay danh sach sinh vien tu file.\n");
	printf("\t9. Thoat ra khoi chuong trinh.\n");
	printf("\n----------------------------------------------------------------------------------\n\n"); 
}  

int main(){
	connectToDB();
	fetchStudents();
	int choice;
	 
	do 
	{
		menu();
		printf("Nhap chuc nang muon thuc hien (0-9): ");
	    while (scanf("%d", &choice) != 1) {
	        printf("Loi! Vui long nhap lai chuc nang la mot so nguyen.\n");
	        while (getchar() != '\n');
			printf("Nhap chuc nang muon thuc hien (1-9): ");
	    }
		printf("\n"); 
		switch(choice){
				case 0: 
				if(isEmpty()){
					printf("Danh sach sinh vien hien dang rong.\n");
				} else {
					deleteAll(); 
				}
				break; 
			case 1:
				addSV();
				break;
			case 2:
		        if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            showDSSV();
		        }
		        break;
			case 3:
		        if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            search();
		        }
		        break;
			case 4:
		        if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            edit();
		        }
		        break;
			case 5:
		        if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            deleted();
		        }
		        break;
			case 6:
				if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            dTBDSSV(); 
		        }
				break;
			case 7:
		        if (isEmpty()) {
		            printf("Danh sach sinh vien hien dang rong.\n");
		        } else {
		            saveFile();
		        }
		        break;				
			case 8:
				readFile();
				break;
			case 9:
				printf("ket thuc chuong trinh. Tam biet!"); 
				break;
			default:	
				printf("Loi! Xin hay nhap lai so tu 0 den 9.\n") 
			;	 
		} 
	}while(choice != 9); 
    closeDB();
    return 0;
}
