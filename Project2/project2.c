#include <stdio.h>
#include <string.h>


void createTask1(){			// hàm tạo task lớn: tạo thêm 1 file mới. Trong file có sẵn description

}

void createTask2(){			// hàm tạo task bé: mở file ghi các task nhỏ
	
}
void addTask(){				// thêm task vào trong file lớn
	// nhập mã task
	
	// mở file
}

void editTask(){
	// nhập mã task
	// mở file
}

void markDone(){
	/* if (completed == 100){
		ask for delete};
	*/
}

void deleteTask(){
	// nhập mã task
	// hỏi trước khi xóa
} 

void findTask(){
	// nhập string (proority, %, ID)
}

void openTask(){
	// nhap ma task
}




int main(){
    printf ("Please choose your option\n");
    printf ("1. Create new task\n");
    printf ("2. Add task to file\n");
    printf ("3. Delete task \n");
    printf ("4. Find task\n");
    printf ("5. Open task\n");
    printf ("6. Edit task\n");
    
    int menuOption = 0;
    scanf ("%d", &menuOption);
    
    if (menuOption == 1){
    	printf ("What task do you want to create?\n");
    	printf ("1. Huge task\n");
    	printf ("2. Task task\n");
    	int taskOption = 0;
    	scanf ("%d", &taskOption);
    	
    	if (taskOption == 1){
    		createTask1();
		}
		else if (taskOption == 2){
			createTask2();
		}
	}
	
	else if(menuOption == 2){
		addTask();
	}
	
	else if(menuOption == 3){
		deleteTask();
	}
    
    else if(menuOption == 4){
		findTask();
	}
	
	else if(menuOption == 5){
		editTask();
	}
        
}


