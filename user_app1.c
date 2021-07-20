#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

#define RD_JIFFIE _IOR('a', 1, unsigned long *)
#define PRINT_JIFFIE _IO('a', 2)

int main(int argc, char const *argv[]) {
  char fname[] = "/dev/ioctl_dev";
  int fd = open(fname, O_RDWR);
  if(fd == -1){
    printf("error opening the file..\n");
    return 0;
  }

  int choice;
  unsigned long jiffie;
  while(1){
    printf("---MENU---\n");
    printf("1. Get the jiffie value from kernel space\n");
    printf("2. Print jiffie value\n");
    printf("3. exit\n");
    printf("Enter your choice:");
    scanf("%d", &choice);

    switch (choice) {
      case 1: if(ioctl(fd, RD_JIFFIE, &jiffie) == -1){
          printf("error in fetching the value\n");
      }else{
        printf("value of jiffie read is: %lu\n", jiffie);
      }
      break;

      case 2: if(ioctl(fd, PRINT_JIFFIE) == -1){
          printf("error in printing\n");
      }else{
        printf("Printed success in kernel space\n");
      }
      break;


      case 3: close(fd);
      return 0;

      default: printf("Enter correct choice\n");
      break;
    }
  }
  return 0;
}
