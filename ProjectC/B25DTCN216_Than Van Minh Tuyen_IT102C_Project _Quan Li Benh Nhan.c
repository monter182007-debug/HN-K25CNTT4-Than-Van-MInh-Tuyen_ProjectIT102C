#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100
#include <ctype.h>

// Khai bao du lieu struct
typedef struct{
	char cardId[10];
	char name[50];
	char phone[15];
	double debt;
	int visitDays;
} Patient;

typedef struct{
	char recId[20];
	char cardId[10];
	char date[20];
	char status[20];
} Record;

// Nguyen mau ham
void printMenu();                   
int checkInput();                   
int isValidDate(char date[]);       
int validateId(char id[]);          
int checkCardId(Patient *patients, int *length, char id[]); 
int checkPhone(char phone[]);       
int checkName(char name[]);         
// Cac chuc nang chinh
void createNewPatient(Patient *patients, int *length);      
void updatePatientInfor(Patient *patients, int length, Record *records, int recordCount); 
void dischargePatient(Patient *patients, int *length);     
void showCurrentPatient(Patient *patients, int length);    
void searchPatient(Patient *patients, int length);      
void sortByDebt(Patient *patients, int length);            

// --- SUADOI: Them tham so *currentRecId vao nguyen mau ham ---
void medicalCheck(Patient *patients, int length, Record *records, int *recordCount, int *currentRecId); 
void viewMedicalHistory(Patient *patients,int length,Record *records,int recordCount);

// Ham main
int main(void){
	Patient patients[MAX];
	int length = 0;
	int choice = 0;
	Record records[MAX];
	int recordCount = 0; 
	// Bien dem ma ho so tu tang
	int currentRecId = 1; 

	do{
		printMenu();
        int validChoice = 0; 
        do {
        	// Validate tranh bi vo han
            printf("Nhap vao lua chon cua ban(0-9): ");
            if (checkInput() == 0) {
                printf("==> Loi: Dinh dang sai (Khong nhap chu/cach/enter)! Nhap lai.\n");
                fflush(stdin); 
            } 
            else {
                scanf("%d", &choice);
                fflush(stdin); 

                if (choice < 1 || choice > 9) {
                    printf("==> Loi: Lua chon phai tu 1 den 9! Nhap lai.\n");
                } else {
                    validChoice = 1; 
                }
            }
        } while (validChoice == 0); 

		switch(choice){
			case 1:{
				createNewPatient(patients, &length); break;
			}
			case 2:{
				updatePatientInfor(patients, length, records, recordCount); 
				break;
			}
			case 3:{
				dischargePatient(patients,&length);
				break;
			}
			case 4:{
				showCurrentPatient(patients,length);
				break;
			}
			case 5:{
				searchPatient(patients,length);
				break;
			}
			case 6:{
				sortByDebt(patients,length);
				break;
			}
			case 7:{
				// --- SUADOI: Truyen dia chi cua currentRecId (&currentRecId) ---
				medicalCheck(patients, length, records, &recordCount, &currentRecId);
				break;
			}
			case 8:{
				viewMedicalHistory(patients,length,records,recordCount);
				break;
			}
			case 9:{
				printf("Thoat chuong trinh!\n");
				break;
			}
		}
		
	} while(choice != 9);
	
	return 0;
}

void printMenu(){
	printf("\n=====================MENU=====================\n");
	printf("|%-44s|\n","1. Tiep nhan benh nhan");
	printf("|%-44s|\n","2. Cap nhat thong tin benh nhan");
	printf("|%-44s|\n","3. Xuat vien");
	printf("|%-44s|\n","4. Hien thi danh sach benh nhan");
	printf("|%-44s|\n","5. Tim kiem benh nhan");
	printf("|%-44s|\n","6. Sap xep danh sach benh nhan");
	printf("|%-44s|\n","7. Ghi nhan kham benh");
	printf("|%-44s|\n","8. Xem lich su kham benh");
	printf("|%-44s|\n","9. Thoat");
	printf("==============================================\n");
}

// Ham ktra ngay thang Case 7
int isValidDate(char date[]){
	if(strlen(date)!=10) {
		return 0;
	}
	
	if(date[2] != '/'|| date[5] !='/'){
		return 0;
	}
	// Khai bao i trong for
	for(int i = 0; i < 10; i++){
		if(i ==2|| i==5){
			 continue;
		}
		if(!isdigit(date[i])){
			return 0;
		}
	}
	int d,m,y;
	// sscanf doc chuoi ky tu
	sscanf(date, "%d/%d/%d", &d, &m, &y); // Tach ngay, thang, nam tu chuoi
	if(y<1900 || y> 2100){
		 return 0;	
	}
	if(m<1|| m> 12){
		return 0;
	}
	// --- SUADOI: Logic ngay thang chuan (maxDay=31) ---
	int maxDay = 31;
	// Cac thang co 30 ngay
	if(m == 4 || m == 6 || m== 9 || m == 11){
		maxDay = 30;
	}else if(m == 2){
		// Logic Nam Nhuan
		if((y % 400 ==0 ) || (y % 4 == 0 && y % 100 != 0)){
			maxDay = 29;
		}else{
			maxDay = 28;
		}
	}
	if(d < 1 || d> maxDay){
		return 0;
	}
	return 1;
}

// Ham ho tro: Validate ID (Chi chua so, khong khoang trang)
int validateId(char id[]){
	if(strlen(id) == 0|| id[0] == ' '){
		printf("Loi: ID khong duoc de trong hoac bat dau bang dau cach!\n");
		return 0;
	}
	// Khai bao i trong for
	for(int i = 0; i < strlen(id); i++){
		if(!isdigit(id[i])){
			printf("Loi: CCCD chi duoc chua so (khong duoc nhap chu va ky tu la)!\n");
            return 0;
		}
	}
	return 1;
}

// Ham ho tro: Kiem tra ID da ton tai trong danh sach chua Case 1
int checkCardId(Patient *patients, int *length, char id[]){
	// Khai bao i trong for
	for(int i = 0; i < *length; i++){
		if(strcmp(patients[i].cardId, id) == 0){
			return 1;
		}
	}
	return 0;
} 

// Ham ho tro: Validate So dien thoai (9-15 so, bat dau bang 0) case 1,2
int checkPhone(char phone[]){
	if(strlen(phone) < 9 || strlen(phone) > 15){
		return 0;
	}
	// Khai bao i trong for
	for(int i = 0; i < strlen(phone); i++){
		if(!isdigit(phone[i])){
			return 0;
		}
	}
	return 1;
}

// Ham ho tro: Validate Ten (Chi chua chu cai va khoang trang) case 1,2
int checkName(char name[]) {
	// Khai bao i trong for
	for (int i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Ham chan cac ky tu dac biet la
int checkInput() {
    char c = getchar(); 
    if (c == '\n'){
    	return 0;
	} 
    if (c == ' '){
    	return 0;
	}
    if (!isdigit(c) && c!='-'){
    	 return 0;
	}
    ungetc(c, stdin); 
    return 1;
}

// Ham nhap suong benh nhan
void createNewPatient(Patient *patients, int *length){
	if(*length >= MAX){
        printf("Danh sach da day (%d/%d), ko the them nua!\n", *length, MAX);
        return;
    }
	
	int size;
    // Vong lap nhap so luong co validate
    do {
        printf("Nhap vao so luong benh nhan muon tiep nhan: ");
        
        if (checkInput() == 0) {
            printf("Loi: Nhap sai dinh dang (Khong Enter, Space, Chu)!\n");
            size = 0;      
            fflush(stdin); 
        } 
        else {
            scanf("%d", &size);
            fflush(stdin);
            
            if (size <= 0) {
                printf("Loi: So luong phai > 0!\n");
            }
            else if (*length + size > MAX) {
                printf("Loi: Khong du cho! Chi con %d cho.\n", MAX - *length);
                size = 0;
            }
        }

    } while (size <= 0);

    // Khai bao i trong for
	for(int i = 0; i < size; i++){
		Patient patient;
		printf("\n+-----Nhap vao thong tin benh nhan thu %d-----+\n", *length + 1);
		
		int validId = 0;
		do {
            printf("Nhap vao CCCD cua benh nhan: ");
            fflush(stdin);
            fgets(patient.cardId, sizeof(patient.cardId), stdin);
            patient.cardId[strcspn(patient.cardId, "\n")] = 0;
            if (validateId(patient.cardId) == 0) {
                validId = 0;
            }
            else if (checkCardId(patients, length, patient.cardId)) {
                printf("Loi: ID nay da ton tai!! Vui long nhap ID khac\n");
                validId = 0;
            }
            else {
                validId = 1;
            }
        } while (!validId);
		
		int validName = 0;
		do{
			printf("Nhap vao ten benh nhan(phai la chu): ");
			fgets(patient.name, sizeof(patient.name), stdin);
			patient.name[strcspn(patient.name, "\n")] = 0;
			if(strlen(patient.name) == 0){
				printf("Loi: Ten benh nhan ko duoc de trong!! Vui long nhap lai\n");
			}else if (patient.name[0] == ' ') {
                printf("Loi: Ten khong duoc bat dau bang dau cach!\n");
            }
			else if(checkName(patient.name) == 0) {
				printf("Loi: Ten chi duoc chua chu cai va khoang trang (khong so, khong ky tu dac biet)!\n");
			}else{
				validName =1;
			}
		} while(!validName);
		
		int validPhone = 0;
		do{
			printf("Nhap vao so dien thoai benh nhan(phai la cac so): ");
			fgets(patient.phone, sizeof(patient.phone), stdin);
			patient.phone[strcspn(patient.phone, "\n")] = 0;
			if(strlen(patient.phone) == 0){
				printf("Loi: SDT ko duoc de trong!!\n");
			} 
			else if(patient.phone[0] != '0'){
				printf("Loi: SDT bat buoc phai bat dau bang so 0 va ko duoc dau cach o dau!\n");
			}
			else if(checkPhone(patient.phone) == 0){
				printf("Loi: SDT phai dai tu 9-15 so va khong chua chu va ky tu la!\n");
			} 
			else {
				validPhone = 1;
		}
		} while(!validPhone);
		int checkDebt; 
        char firstChar; 

        do {
        	patient.debt = -1;
            printf("Nhap vao cong no hien tai cua benh nhan(phai la cac so): ");
            
            fflush(stdin); 
            firstChar = getchar();

            if (firstChar == '\n') {
            
                printf("Loi: Khong duoc de trong! Vui long nhap tien.\n");
                patient.debt = 0; 
            }
            else if (firstChar == ' ') {
	
                printf("Loi: Khong duoc bat dau bang dau cach!\n");
                patient.debt = 0;
            }
            else if (!isdigit(firstChar) && firstChar != '-') {
    
                printf("Loi: Ban da nhap chu! Vui long chi nhap so .\n");
                patient.debt = 0;
            }
            else {
              
                ungetc(firstChar, stdin); 
                
                checkDebt = scanf("%lf", &patient.debt);
            	if (checkDebt != 1) {
        			printf("Loi: Nhap sai dinh dang so!\n");
        			patient.debt = -1;
					fflush(stdin);
					
    			}else if (patient.debt < 0) {
        			printf("Loi: Cong no phai la so duong!!!\n");
    			}

            }
            if(firstChar != '\n') fflush(stdin);

        } while (patient.debt <= 0);
	
		patient.visitDays = 0;
		patients[*length] = patient; 
		(*length)++; 
		
		printf("Da them thanh cong benh nhan!!!\n");
	}
}

// CASE 2: CAP NHAT THONG TIN 
void updatePatientInfor(Patient *patients, int length, Record *records, int recordCount){
	if(length<= 0){
		printf("Danh sach benh nhan trong! Vui long nhap so luong benh nhan\n");
		return;
	}
	char id[10]; 
	int foundIndex = -1;
	int isValid; 
    do {
        printf("Nhap CCCD benh nhan can sua thong tin: ");
        fflush(stdin);
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = 0;
        if (validateId(id) == 1) {
            isValid = 1; 
        } else {
            isValid = 0; 
        }
    } while (isValid == 0);
	

	for(int i = 0; i < length; i++){
		if(strcmp(patients[i].cardId, id) == 0){
			foundIndex = i;
			break;
		}
	}
	
	if(foundIndex == -1){
		printf("Loi: Khong tim thay benh nha co ma %s can cap nhat\n", id);
		return;
	}
	
	// --- CHECK LOGIC: Kiem tra xem benh nhan da Xuat Vien chua ---
	char tempStatus[20];

	for(int i = 0; i < recordCount; i++){
		if(strcmp(records[i].cardId, id) == 0) {
			
			strcpy(tempStatus,records[i].status);
			// Chuyen chuoi ve chu thuong de so sanh (vd: "Xuat Vien" -> "xuat vien")

			for(int j = 0; j < strlen(tempStatus); j++){
				if(tempStatus[j]>='A' && tempStatus[j]<='Z'){
					tempStatus[j]+=32;
				}
			}
			if(strcmp(tempStatus,"xuat vien") == 0){
				printf("Canh bao: Benh nhan nay da xuat vien (Trang thai: %s). Khong duoc phep chinh sua\n", records[i].status);
			    return;
			}
		}
	}
	printf("Da tim thay: %s (Enter de giu nguyen thong tin cu)\n", patients[foundIndex].name);
	
	char buffer[50];
	printf("1. Nhap ten moi: ");
	fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = 0;
	if(strlen(buffer) > 0){
		if (buffer[0] == ' ') {
            printf("Loi: Ten khong duoc bat dau bang dau cach! -> Giu nguyen ten cu.\n");
        }
        else if (checkName(buffer) == 0) {
            printf("Loi: Ten chua ky tu khong hop le (so/ky tu la)! -> Giu nguyen ten cu.\n");
        }
        else {
            strcpy(patients[foundIndex].name, buffer);
            printf("-> Da cap nhat Ten thanh cong.\n");
        }
	}
	
	int phoneOk = 0;
    do {
        printf("2. Nhap so dien thoai moi(Nhan Enter de giu nguyen): ");	
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            phoneOk = 1;
        } 
        else if (buffer[0] != '0') {
             printf("Loi: SDT bat buoc phai bat dau bang so 0 va ko duoc dung dau cach o dau!\n");
        }
        else if (checkPhone(buffer) == 0) {
             printf("Loi: SDT phai la so duong , do dai tu 9-15 va` khong duoc chua chu va ky tu la!\n");
        }
        else {
             strcpy(patients[foundIndex].phone, buffer);
             phoneOk = 1;
             printf("Cap nhat SDT thanh cong!\n");
        }
    } while (!phoneOk);
    
    double newDebt;
    int debtOk = 0;
    char firstChar; 
    int checkScanf;

    do {
        printf("3. Nhap Cong no moi (Nhap -1 de giu nguyen): ");
        fflush(stdin);
        firstChar = getchar();

        if (firstChar == '\n') {
            printf("-> Giu nguyen cong no cu.\n");
            debtOk = 1; 
        }
        else if (firstChar == ' ') {
            printf("Loi: Khong duoc bat dau bang dau cach!\n");
        }
        else if (!isdigit(firstChar) && firstChar != '-') {
            printf("Loi: Ban da nhap chu! Vui long chi nhap so .\n");
        }
        else {
            
            ungetc(firstChar, stdin);
            checkScanf = scanf("%lf", &newDebt);
            
            if (checkScanf == 0) {
                 printf("Loi: Dinh dang so khong hop le!\n");
            }
            else {
                 if (newDebt == -1) {
                    printf("-> Giu nguyen cong no cu.\n");
                    debtOk = 1;
                 }
                 else if (newDebt < 0) {
                    printf("Loi: Cong no phai la so duong (hoac -1 de bo qua)!\n");
                 }
                 else {
                    patients[foundIndex].debt = newDebt;
                    printf("-> Cap nhat Cong no thanh cong.\n");
                    debtOk = 1;
                 }
            }
        }
        if(firstChar != '\n') fflush(stdin);

    } while (!debtOk);
    
    printf("Cap nhat thong tin hoan tat!\n");
}

// --- CASE 3: XUAT VIEN (Xoa benh nhan khoi mang) ---
void dischargePatient(Patient *patients,int *length){
	if(*length<= 0){
		printf("Danh sach benh nhan trong! Vui long nhap so luong benh nhan\n");
		return;
	}
	char id[10];
	int foundIndex = -1;

	int validId;
	do{
		printf("Nhap vao CCCD benh nhan can xuat vien: ");
		fgets(id,sizeof(id),stdin);
		id[strcspn(id,"\n")] = 0;
		if(validateId(id) == 1){
			validId =1;
		}else{
			validId = 0;
		}
	}while(validId == 0);
	
	for(int i = 0; i < *length; i++){
		if(strcmp(patients[i].cardId,id) == 0){
			foundIndex = i;
			break;
		}
	}
	if(foundIndex == -1){
		printf("Loi: Ko tim thay benh nhan co ma %s can xuat vien\n",id);
		return;
	}
	// Check No truoc khi xoa
	if(patients[foundIndex].debt > 0){
		printf("CANH BAO: Benh nhan con no %.0lf VND.\n", patients[foundIndex].debt);
		printf("Yeu cau thanh toan het vien phi truoc khi xuat vien!\n");
		return ;
	}
	// Logic xoa phan tu khoi mang
	for(int i = foundIndex; i < *length-1; i++){
		patients[i] = patients[i+1];
	}
	(*length)--;
	printf("-> Xuat vien THANH CONG! Da xoa ho so cua benh nhan khoi he thong.\n");
}

// --- CASE 4: HIEN THI DANH SACH (Phan trang) ---
void showCurrentPatient(Patient *patients,int length){
	if(length <= 0){
		printf("Thong bao: Danh sach hien dang trong!!\n");
		return;
	}
	int pageSize = 10;
	int totalPages =  (length + pageSize -1 )/ pageSize;
	int currentPage = 1;//Mac dinh hien trang ban dau 1 ngay
	int targetPage; // i da chuyen vao trong for
	do{
		int startIndex = (currentPage -1 )*pageSize;
		int endIndex = pageSize + startIndex;
		
		if(endIndex > length){
			endIndex = length;
		}
		printf("\n=================DANH SACH BENH NHAN(Trang %d / %d)================================================\n",currentPage,totalPages);
		printf("+------------+---------------------------+---------------+---------------+----------------------+\n");
		printf("| %-10s | %-25s | %-13s | %-13s | %-20s |\n", 
               "CCCD", "HO TEN", "SDT", "CONG NO", "SO NGAY KHAM BENH");
        printf("+------------+---------------------------+---------------+---------------+----------------------+\n");
        // Khai bao i trong for
		for(int i = startIndex; i < endIndex; i++){
			printf("| %-10s | %-25s | %-13s | %-13.0lf | %-20d |\n", 
                   patients[i].cardId, 
                   patients[i].name, 
                   patients[i].phone, 
                   patients[i].debt, 
                   patients[i].visitDays);
            
            printf("+------------+---------------------------+---------------+---------------+----------------------+\n");
        }
        printf("\nTong cong: %d benh nhan.\n", length);
        printf("Ban muon xem trang may? (Nhap 0 de thoat): ");
        if(checkInput() == 0){
        	printf("Loi: Nhap sai dinh dang! Vui long chi nhap so (Ko nhap Enter/Space/ Ky tu la!)");
        	fflush(stdin);
        	continue;
		}else{
			scanf("%d",&targetPage);
			fflush(stdin);
		}
		if(targetPage == 0){
			printf("-> Thoat che do xem!\n");
			return;
		}else if(targetPage < 1|| targetPage > totalPages){
			printf("(!) Loi: Trang %d khong ton tai!\n", targetPage);
            printf("-> He thong se hien thi lai trang hien tai.\n");
		}else{
			currentPage = targetPage;
		}
	}while(1);
}

// --- CASE 5: TIM KIEM BENH NHAN ---
void searchPatient(Patient *patients,int length){
	if(length<= 0){
		printf("Danh sach benh nhan trong! Vui long nhap so luong benh nhan\n");
		return;
	}
	char name[50];
	// i, j da chuyen vao trong for
	int validName = 0;
	int found =0;
	do{
		printf("Nhap vao ten benh nhan can tim kiem: ");
		fgets(name,sizeof(name),stdin);
		name[strcspn(name,"\n")] = 0;
		if(strlen(name) == 0){
			printf("Loi: Ten ko duoc de trong! Vui long nhap lai\n");
		}else if(name[0] == ' '){
			printf("Loi: Ten ko duoc bat dau bang dau cach\n");
		}else if(checkName(name) == 0){
			printf("Loi: Ten chi duoc la chu cai va khoang trang(ko so ,ko ky tu dac biet)\n");
		}else{
			validName = 1;
		}
	}while(!validName);
	printf("\nKet qua tim kiem cho tu khoa: \"%s\"\n", name);
	char nameTemp[50]; // Luu tam ten benh nhan
	char searchTemp[50]; // Luu tam ten tim kiem
	strcpy(searchTemp,name);
	// Chuyen tu khoa tim kiem ve chu thuong
	// Khai bao i trong for
	for(int i = 0; i < strlen(searchTemp); i++){
		if(searchTemp[i] >= 'A' && searchTemp[i]<= 'Z'){
			searchTemp[i] += 32; 
		}
	}
	int isHeader = 0;
	// Khai bao i trong for
	for(int i = 0; i < length; i++){
		strcpy(nameTemp,patients[i].name);
		// Chuyen ten trong danh sach ve chu thuong
		// Khai bao j trong for
		for(int j = 0; j < strlen(nameTemp); j++){
			if(nameTemp[j] >= 'A'&& nameTemp[j]<='Z'){
				nameTemp[j] +=32;
		   }	
		}
		// Dung strstr de tim chuoi con
		if(strstr(nameTemp,searchTemp)!= NULL){
			if(isHeader == 0){
				printf("+------------+---------------------------+---------------+---------------+----------+\n");
				printf("| %-10s | %-25s | %-13s | %-13s | %-8s |\n", 
               "CCCD", "HO TEN", "SDT", "CONG NO", "SO NGAY");
    			printf("+------------+---------------------------+---------------+---------------+----------+\n");
    			isHeader = 1;
			}
			found = 1;
			printf("| %-10s | %-25s | %-13s | %-13.0lf | %-8d |\n", 
                   patients[i].cardId, 
                   patients[i].name, 
                   patients[i].phone, 
                   patients[i].debt, 
                   patients[i].visitDays);
	 		printf("+------------+---------------------------+---------------+---------------+----------+\n");		
		}
	}
	if(found == 0){
		printf("Ko tim thay benh nhan nao trong danh sach\n!!");
	}
}

// --- CASE 6: SAP XEP DANH SACH ---
void sortByDebt(Patient *patients,int length){
	if(length <=0 ){
		printf("Danh sach benh nhan hien dang trong!!\n");
		return;
	}
	int sortChoice = 0;
	// i, j da chuyen vao trong for
	int validChoice = 0;
	printf("1. Sap xep tang dan theo cong no\n");
	printf("2. Sap xep giam dan theo cong no\n");
	do{
		printf("Nhap vao lua chon cua ban(1-2): ");
		if(checkInput() == 0){
			printf("==> Loi: Dinh dang sai (Khong nhap chu/cach/enter)! Nhap lai.\n");
            fflush(stdin); 
		}else{
			scanf("%d",&sortChoice);
			fflush(stdin);
			if(sortChoice == 1|| sortChoice == 2){
				validChoice = 1;
			}else{
				printf("==> Loi: Chi duoc nhap so 1 hoac 2! Vui long nhap lai.\n");
                validChoice = 0;
			}
		}
	}while(validChoice == 0);
	switch(sortChoice){
		case 1:{
			// Khai bao i, j trong for
			for(int i = 0; i < length-1; i++){
				for(int j = 0; j < length -1-i; j++){
					if(patients[j].debt > patients[j+1].debt){
						Patient tmp = patients[j];
						patients[j] = patients[j+1];
						patients[j+1] = tmp;
					}
				}
			}
			break;
		}
		case 2:{
			// Khai bao i trong for
			for(int i = 0; i < length-1; i++){
				for(int j = 0; j < length -1-i; j++){
					if(patients[j].debt < patients[j+1].debt){
						Patient tmp = patients[j];
						patients[j] = patients[j+1];
						patients[j+1] = tmp;
					}
				}
			}
			break;
		}
	}
	printf("+------------+---------------------------+---------------+---------------+----------+\n");
	printf("| %-10s | %-25s | %-13s | %-13s | %-8s |\n", 
            "CCCD", "HO TEN", "SDT", "CONG NO", "SO NGAY");
    printf("+------------+---------------------------+---------------+---------------+----------+\n");
    // Khai bao i trong for
    for(int i = 0; i < length; i++){
    	printf("| %-10s | %-25s | %-13s | %-13.0lf | %-8d |\n", 
                   patients[i].cardId, 
                   patients[i].name, 
                   patients[i].phone, 
                   patients[i].debt, 
                   patients[i].visitDays);
	}
	printf("+------------+---------------------------+---------------+---------------+----------+\n");
	printf("Da sap xep theo cong no thanh cong!!!!\n");
}

// --- CASE 7: GHI NHAN KHAM BENH (DA SUA LOGIC TU TANG ID) ---
void medicalCheck(Patient *patients,int length,Record *records,int *recordCount, int *currentRecId){
	if(*recordCount >= MAX){
		printf("Loi: Danh sach ho so kham da day ,ko the them moi!!\n");
		return;
	}
	char id[10];
	int foundIndex = -1;
	// i da chuyen vao trong for
	int validId = 0;
	do{
		printf("\n--- GHI NHAN KHAM BENH ---\n");
		printf("Nhap vao CCCD benh nhan: ");
		fflush(stdin);
		fgets(id,sizeof(id),stdin);
		id[strcspn(id,"\n")] = 0;
		if(validateId(id) == 0){
			validId = 0;
		}else{
			foundIndex = -1;
			// Khai bao i trong for
			for(int i = 0; i < length; i++){
				if(strcmp(patients[i].cardId,id) == 0){
					foundIndex = i;
					break;
				}
			}
			if(foundIndex == -1){
				printf("Loi: Khong tim thay benh nhan co CCCD %s trong he thong!\n", id);
				return;
			}else{
				validId = 1;
				printf("-> Da tim thay benh nhan: %s\n", patients[foundIndex].name);
			}
		}
	}while(!validId);
	
	// --- SUADOI: LOGIC TU TANG MA HO SO ---
	char autoRecId[20];
	// Tu dong tao ma dang "HS001", "HS002"... dua tren currentRecId
	sprintf(autoRecId, "HS%03d", *currentRecId);
	printf("-> He thong da tu dong tao ma ho so: %s\n", autoRecId);
	
	// Khong can loop nhap ma va check trung lap nua vi ma tu tang la duy nhat
	
	char inputDate[20];
    int validDate = 0;
    do{
    	printf("Nhap vao ngay kham(dd/mm/yyyy):  ");
    	fflush(stdin);
    	fgets(inputDate,sizeof(inputDate),stdin);
    	inputDate[strcspn(inputDate,"\n")] = 0;
    	
    	if(strlen(inputDate) == 0){
    		printf("Loi: Ngay kham khong duoc de trong!\n");
            validDate = 0;
		}else if(isValidDate(inputDate) == 0){
			printf("Loi: Ngay thang khong hop le!\n");
    		printf("-> Yeu cau: Dinh dang dd/mm/yyyy, nam tu 1900-2100, ngay phai ton tai.\n");
            validDate = 0;
            
		}else{
			int checkId = 0;
			// Khai bao i trong for
			for(int i = 0; i < *recordCount; i++){
				// Check Logic: Trung ID benh nhan VA Trung ngay kham -> Bao loi
				if(strcmp(records[i].cardId,id) == 0&& strcmp(records[i].date,inputDate) == 0){
					checkId = 1;
					break;
				}
			}
			if(checkId == 1){
				printf("Loi: Benh nhan %s da co lich kham ngay %s roi!\n", id, inputDate);
				validDate = 0;// Bat nhap lai
			}else{
				validDate = 1;//Hop le
			}
		}
	}while(!validDate);
	
	char inputStatus[20];
    int validStatus = 0;
    
    do {
        printf("Nhap trang thai (vd: Tai kham, Theo doi...): ");
        fflush(stdin);
        fgets(inputStatus, sizeof(inputStatus), stdin);
        inputStatus[strcspn(inputStatus, "\n")] = 0; 

        if (strlen(inputStatus) == 0||inputStatus[0] == ' ') {
            printf("Loi: Trang thai khong duoc de trong hoac co dau cach dau dong! Vui long nhap lai.\n");
            validStatus = 0;
        } else {
            validStatus = 1;
        }
    } while (!validStatus);
	
	// Gan gia tri tu dong vao struct
	strcpy(records[*recordCount].recId, autoRecId);
    strcpy(records[*recordCount].cardId, id);
    strcpy(records[*recordCount].date, inputDate);
    strcpy(records[*recordCount].status, inputStatus);
    
    patients[foundIndex].visitDays++;
    (*recordCount)++;
    
    // --- SUADOI: Tang bien dem len 1 don vi cho lan sau ---
    (*currentRecId)++;
    
    printf("Ghi nhan lich kham (Ma: %s) ngay %s cho benh nhan %s thanh cong.\n", autoRecId, inputDate, id);
}

void viewMedicalHistory(Patient *patients, int length, Record *records, int recordCount){
    if(length <= 0){
        printf("Danh sach benh nhan trong!!!\n");
        return;
    }
    char id[10];
    int foundIndex = -1;
    int validId = 0;
    
    // 1. Doan nhap ID giu nguyen
    do{
        printf("\n--- XEM LICH SU KHAM BENH ---\n");
        printf("Nhap CCCD benh nhan can xem: ");
        fflush(stdin);
        fgets(id,sizeof(id),stdin);
        id[strcspn(id, "\n")] = 0;

        if (validateId(id) == 0) {
            validId = 0;
        } else {
            foundIndex = -1;
            for (int i = 0; i < length; i++) {
                if (strcmp(patients[i].cardId, id) == 0) {
                    foundIndex = i;
                    break;
                }
            }
            if (foundIndex == -1) {
                printf("Loi: Khong tim thay benh nhan co CCCD %s!\n", id);
                return; 
            } else {
                validId = 1;
                printf("-> Lich su kham cua benh nhan: %s (CCCD: %s)\n", patients[foundIndex].name, id);
            }
        }
    } while (!validId);

    // 2. LOGIC MOI: Chi in bang khi co du lieu
    int foundRecord = 0; // Bien nay vua la co (flag), vua de dem
    
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].cardId, id) == 0) {
            
            // Neu day la lan dau tien tim thay (foundRecord van la 0) -> IN TIEU DE
            if(foundRecord == 0){
                printf("\n+----------------------+----------------------+---------------------------+\n");
                printf("| %-20s | %-20s | %-25s |\n", "NGAY KHAM", "MA HO SO", "TRANG THAI");
                printf("+----------------------+----------------------+---------------------------+\n");
            }
            
            // Sau do in du lieu ra
            printf("| %-20s | %-20s | %-25s |\n", records[i].date, records[i].recId, records[i].status);
            printf("+----------------------+----------------------+---------------------------+\n");
            
            // Danh dau la da tim thay it nhat 1 cai
            foundRecord = 1;
        }
    }

    // 3. Neu chay het vong lap ma foundRecord van = 0 -> Tuc la khong co lich su nao
    if (foundRecord == 0) {
        // Chi in dong thong bao, khong in bang ke gi ca
        printf("\n=> Thong bao: Benh nhan nay CHUA CO lich su kham benh nao!\n");
    }
}
