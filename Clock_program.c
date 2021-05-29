#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bmpio.h>

unsigned char pic[2000][2000][3];
unsigned char crop[2000][2000][3];
unsigned char pic2[2000][2000][3];
unsigned int avg[400][300][3];

void first_crop(int height,int width,int*h,int*w);
void khat_boresh(int satr[],int sotoon[],int*v,int*c,int h , int w);
void final_crop (int c, int v, int satr[],int sotoon[],int height,int width,int karkard);
void crop_func(int height,int width,int start_sa , int start_so);
void copy_crop_to_pic(int height,int width);
int up=0,down=0,left=0,right=0;

void resize_check(double nesbat_w ,double nesbat_h , int w_size , int h_size,char name[],int karkard);
void width_up(double nesbat_w,int height,int width);
void height_up(double nesbat_h,int height,int new_width);
void width_down(double nesbat_w,int height,int width);
void height_down(double nesbat_h,int height,int new_width);
void DataSetAvg(int karkard);
void read_clock();
int menu();

int main(){
	system ("color 3E");//color of cmd
	printf("\n\t\t\t***  WELCOME TO MY PROGRAM  ***\n\n");
	while(1){
		int karkard; 
		karkard = menu();
		if(karkard==0){
			return 0;
		}
		if(karkard==4){
			DataSetAvg(karkard);	
		}
		
		else{
			printf("Enter the address of photo:\n");
			char path[100];
			scanf("%s",path);
			int height,width;
			readBMP(path,&width,&height,pic);
			
			if(karkard==1){
				int h=0,w=0;//baraye zakhire height and width baed az crop
				/*crop avaliye*/
				first_crop(height,width,&h,&w);
				int satr[20] , sotoon[20]; //baraye zakhire mokhtasat boresh ha
				int v=1,c=1;// shomare khoone araye
				/*peyda kardan khat boresh*/
				khat_boresh(satr,sotoon,&v,&c,h,w);
				/*final crop*/
				final_crop(c,v, satr,sotoon,height,width,karkard);
				printf("\t\t\t\t      Done!\n\n");
			}
			if(karkard==2){
				int new_height,new_width;
				printf("\nEnter new width: ");
				scanf("%d",&new_width);
				printf("\nEnter new height: ");
				scanf("%d",&new_height);
				double nesbat_h = 1.0 * (new_height)/height,nesbat_w = 1.0 * (new_width)/width;
				char name[] = "Scaled photo.bmp";
				resize_check(nesbat_w,nesbat_h , width , height,name,karkard);
				printf("\t\t\t\t      Done!\n\n");
			}
			if(karkard==3){
				int h=0,w=0; //baraye zakhire height and width baed az crop
				/*crop avaliye*/
				first_crop(height,width,&h,&w);
				/*peyda kardan khat boresh*/
				int satr[20] , sotoon[20]; //baraye zakhire mokhtasat boresh ha
				int v=1,c=1;// shomare khoone araye
				khat_boresh(satr,sotoon,&v,&c,h,w);
				/*final crop and resize*/
				final_crop(c,v, satr,sotoon,height,width,karkard);
				int hour_result[9] = {0};
				read_clock(hour_result);
				//graphic part
				digital_clock(hour_result);
				analog_clock(hour_result);
				printf("\t\t\t\t      Done!\n\n");
			}
		}
	}
    return 0;
}
int menu(){
	int karkard;
	printf("\t\t\t        For Crop Press 1 \n\n");
	printf("\t\t\t       For Resize Press 2 \n\n");
	printf("\t       For Read Clock if you have provided the DataSet Press 3 \n\n");
	printf("\t\t\t  For Providing DataSet Press 4 \n\n");
	printf("\t\t\t\tFor Exit Press 0 \n");
	printf("\n\t\t\t    Please Enter Your Choice: ");
	scanf("%d",&karkard);
	return karkard;
}

void first_crop(int height,int width,int*h,int*w){
	*h=0;*w=0;
	int i,j,shart=0,color;
	for(i=0;i<height && shart==0;i++){
		for(j=0;j<width;j++){
			if(!(pic[i][j][0] == 255 && pic[i][j][1] == 255 && pic[i][j][2] == 255)){
				up = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=height-1;i>=0 && shart==0;i--){
		for(j=width-1;j>=0;j--){
			if(!(pic[i][j][0] == 255 && pic[i][j][1] == 255 && pic[i][j][2] == 255)){
				down = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=0;i<width && shart==0;i++){
		for(j=0;j<height;j++){
			if(!(pic[j][i][0] == 255 && pic[j][i][1] == 255 && pic[j][i][2] == 255)){
				left = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=width-1;i>=0 && shart==0;i--){
		for(j=height-1;j>=0;j--){
			if(!(pic[j][i][0] == 255 && pic[j][i][1] == 255 && pic[j][i][2] == 255)){
				right = i;
				shart=1;
				break;
			}
		}	
	}
	//sakht araye ax crop shode
	for(i=up;i<=down && up != down;i++){
		for(j=left;j<=right && left != right;j++){
			for(color=0;color<3;color++){
				crop[i-up][j-left][color] = pic[i][j][color];
			}
		}
	}
	*w = right - left ; // width ax crop shode
	*h = down - up ;	// height ax crop shode
}
void copy_crop_to_pic(int height,int width){//copy crop array into pic array
	int i,j,color;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			for(color=0;color<3;color++){
				pic[i][j][color] = crop[i][j][color];
			}
		}
	}
}
void khat_boresh(int satr[],int sotoon[],int*v,int*c,int h , int w){
	int count=0,flag=0,i,j;
	satr[0]=0;
	sotoon[0]=0;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(crop[i][j][0] == 255 && crop[i][j][1] == 255 && crop[i][j][2] == 255)
				count++;        
			else{
				flag=1; 
				break;
				/* dar sorat moshahede rangi gheir az sefid*/
			}
		}
		if(count==w && flag==1){
			/*shart flag==1 be in khater ast ke zamanike
			chand khat sefid bein 2 shekl hast tanha avali
			ra dar nazar begirad*/
			satr[*v]= i;
			(*v)++;
			flag=0;
		}
		count=0;
	}
	count = 0;flag=0;
	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			if(crop[j][i][0] == 255 && crop[j][i][1] == 255 && crop[j][i][2] == 255)
				count++;
			else{
				flag=1;
				break;
			}
				
		}
		if(count==h && flag==1){
			flag=0;	
			sotoon[*c]= i;
			(*c)++;
		}
		count=0;
	}
	satr[*v]=h;
	sotoon[*c]=w;
}	
void final_crop (int c, int v, int satr[],int sotoon[],int height,int width,int karkard){
	int i,j,h_size=0,w_size=0,count=1,g,b,color;
	
	char name[10];
	char name1[10];
	for(i=0;i<v;i++){
		for(j=0;j<c;j++){
			crop_func(satr[i+1],sotoon[j+1],satr[i],sotoon[j]); // crop 1 shekl
			//sakht araye ax crop shode
			for(g=up;g<=down && up != down;g++){
				for(b=left;b<=right && right != left;b++){
					for(color=0;color<3;color++){
						pic[g-up][b-left][color] = crop[g][b][color];
					}
				}
			}
			w_size = right - left;  // width ax crop shode
			h_size = down - up;		// height ax crop shode
			double nesbat_h = 1.0* 400/h_size, nesbat_w = 1.0 * 300/w_size;
			//namgozari
			sprintf(name1,"crop%d.bmp",count);
			sprintf(name,"resize%d.bmp",count);
			count++;
			if(w_size >0 && h_size >0 ){//dar sorati ke kol ax sefid nabashad
				if(karkard==1){
					saveBMP(pic,w_size,h_size,name1);	
				}
				else if(karkard==3){
					resize_check(nesbat_w , nesbat_h , w_size , h_size,name,karkard);
				}
			}
		}
	}
}
void crop_func(int height,int width,int start_sa , int start_so){
	int i,j,shart=0;
	up=0,down=0,left=0,right=0;
	for(i=start_sa;i<height && shart==0;i++){
		for(j=start_so;j<width;j++){
			if(!(crop[i][j][0] == 255 && crop[i][j][1] == 255 && crop[i][j][2] == 255)){
				up = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=height-1;i>=start_sa && shart==0;i--){
		for(j=width-1;j>=start_so;j--){
			if(!(crop[i][j][0] == 255 && crop[i][j][1] == 255 && crop[i][j][2] == 255)){
				down = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=start_so;i<width && shart==0;i++){
		for(j=start_sa;j<height;j++){
			if(!(crop[j][i][0] == 255 && crop[j][i][1] == 255 && crop[j][i][2] == 255)){
				left = i;
				shart=1;
				break;
			}
		}	
	}
	shart=0;
	for(i=width-1;i>=start_so && shart==0;i--){
		for(j=height-1;j>=start_sa;j--){
			if(!(crop[j][i][0] == 255 &&crop[j][i][1] == 255 && crop[j][i][2] == 255)){
				right = i;
				shart=1;
				break;
			}
		}	
	}
}
void resize_check(double nesbat_w , double nesbat_h , int w_size , int h_size,char name[],int karkard){
	if(nesbat_w>=1){
		width_up(nesbat_w,h_size,w_size);
	}
	if(nesbat_w<1){
		width_down(nesbat_w,h_size,w_size);
	}
	if(nesbat_h>=1){
		height_up(nesbat_h,h_size,nesbat_w*w_size);
	}
	if(nesbat_h<1){
		height_down(nesbat_h,h_size,nesbat_w*w_size);
	}
	if(karkard != 4 && nesbat_w*w_size > 0 && nesbat_h*h_size>0){
		saveBMP(pic,nesbat_w*w_size,nesbat_h*h_size,name);	
	}	
}
void width_up(double nesbat_w,int height,int width){
	int i,j,m,shart;
	double nesbat;
	nesbat = nesbat_w;
	for(i=0;i<height;i++){
		m=0;//shomare pixel ax resize shode
		for(j=0;j<width;j++){
			shart=1; // shart khoroj az halghe while
			if(nesbat==0){
				nesbat = nesbat_w;
			}
			while(nesbat>0 && shart!=0){
				if(nesbat>=1){
					pic2[i][m][0] = pic[i][j][0];
					pic2[i][m][1] = pic[i][j][1];
					pic2[i][m][2] = pic[i][j][2];
					m++;
					nesbat--;
				}
				else if(nesbat<1){
					pic2[i][m][0] = (pic[i][j][0] * nesbat) +( (1-nesbat) * pic[i][j+1][0] );
					pic2[i][m][1] = (pic[i][j][1]* nesbat) +( (1-nesbat) * pic[i][j+1][1] );
					pic2[i][m][2] = (pic[i][j][2]* nesbat) +( (1-nesbat) * pic[i][j+1][2] );
					m++;
					nesbat = nesbat_w - (1-nesbat);
					shart=0;
				}
			}
		}
	}
}
void height_up(double nesbat_h,int height,int new_width){
	int i,j,m,shart;
	double nesbat;
	nesbat = nesbat_h;
	for(i=0;i<new_width;i++){
		m=0; //shomare pixel ax resize shode
		for(j=0;j<height;j++){
			shart=1; //shart khoroj az halghe while
			if(nesbat==0){
				nesbat = nesbat_h;
			}
			while(nesbat>0 && shart != 0){
				if(nesbat>=1){
					pic[m][i][0] = pic2[j][i][0];
					pic[m][i][1] = pic2[j][i][1];
					pic[m][i][2] = pic2[j][i][2];
					m++;
					nesbat--;
				}
				else if(nesbat<1){
					pic[m][i][0] = (pic2[j][i][0] * nesbat) +( (1-nesbat) * pic2[j][i+1][0] );
					pic[m][i][1] = (pic2[j][i][1]* nesbat) +( (1-nesbat) * pic2[j][i+1][1] );
					pic[m][i][2] = (pic2[j][i][2]* nesbat) +( (1-nesbat) * pic2[j][i+1][2] );
					m++;
					nesbat = nesbat_h - (1-nesbat);
					shart=0;
				}
			}
		}
	}
}
void width_down(double nesbat_w,int height,int width){
	int i,j,r,m;
	double nesbat=0,nesbat2=0,sumR=0,sumG=0,sumB=0,sum_nesbat=0,sum_nesbat2=0;
	//nesbat va sum_nesbat = marboot be nesbat voroodi karbar
	/* nesbat2 va sum_nesbat2 = marboot be bakhsi az 1 pixel az aks voroodi
	 ke dar ax resize shode dar 2 pixel gharar migirad*/
	for(i=0;i<height;i++){
		m=0; //shomare pixel ax resize shode	
		for(j=0;j<width;){
			if(nesbat==0){
				nesbat = nesbat_w;
			}
			while(sum_nesbat<0.99){    //sum_nesbat<1 , be dalil khata mohasebe aeshari <0.99
				sum_nesbat += nesbat;
				if(sum_nesbat>1){
					nesbat2 = 1-(sum_nesbat-nesbat_w);
				}
				if( sum_nesbat > 0.99 && sum_nesbat<=1){ //sum_nesbat==1 , be dalil khata mohasebe aeshari
					nesbat2 = 0;
				}

				if(nesbat2 == 0){
					sumR += (pic[i][j][0] * nesbat);//jame rang red
					sumG += (pic[i][j][1]* nesbat);//jame rang green
					sumB += (pic[i][j][2]* nesbat);//jame rang blue
					j++;
				}
				//zamanike 1 pixel ax voroodi dar 2 pixel ax resize shode gharar migirad
				else{
					if(sum_nesbat2>0){
						r = j-1;
						j--;
					}
					else {
						r=j;
					}
					
					sumR += (pic[i][r][0] * nesbat2);
					sumG += (pic[i][r][1]* nesbat2);
					sumB += (pic[i][r][2]* nesbat2);
					j++;
					if(nesbat-nesbat2>0){
						sum_nesbat = sum_nesbat -nesbat + nesbat2;
						nesbat2=nesbat-nesbat2;
						sum_nesbat2 += 	nesbat2;
						
					}
					else{
						nesbat2=0;
					}
				}
				if(sum_nesbat2 == nesbat){
					nesbat2=0;
					sum_nesbat2=0;
				}
			}
			pic2[i][m][0] = sumR; 
			pic2[i][m][1] = sumG; // sakht araye ax resize shode
			pic2[i][m][2] = sumB;
			m++;
			sumR=0;sumG=0;sumB=0;
			sum_nesbat=0;
		}
	}
}
void height_down(double nesbat_h,int height,int new_width){
	int i,j,r,m;
	double nesbat=0,nesbat2=0,sumR=0,sumG=0,sumB=0,sum_nesbat=0,sum_nesbat2=0;
	//nesbat va sum_nesbat = marboot be nesbat voroodi karbar
	/* nesbat2 va sum_nesbat2 = marboot be bakhsi az 1 pixel az aks voroodi
	 ke dar ax resize shode dar 2 pixel gharar migirad*/
	for(i=0;i<new_width;i++){
		m=0; //shomare pixel ax resize shode	
		for(j=0;j<height;){
			if(nesbat==0){
				nesbat = nesbat_h;
			}
			while(sum_nesbat<0.99){  //sum_nesbat<1 , be dalil khata mohasebe aeshari <0.99
				sum_nesbat += nesbat;
				if(sum_nesbat>1){
					nesbat2 = 1-(sum_nesbat-nesbat_h);
				}
				if( sum_nesbat > 0.99 && sum_nesbat<=1){ //sum_nesbat==1 , be dalil khata mohasebe aeshari
					nesbat2 = 0;
				}
				if(nesbat2 == 0){
					sumR += (pic2[j][i][0] * nesbat);
					sumG += (pic2[j][i][1]* nesbat);
					sumB += (pic2[j][i][2]* nesbat);
					j++;
				}
				//zamanike 1 pixel ax voroodi dar 2 pixel ax resize shode gharar migirad
				else{
					if(sum_nesbat2>0){
						r = j-1;
						j--;
					}
					else {
						r=j;
					}
					
					sumR += (pic2[r][i][0] * nesbat2);
					sumG += (pic2[r][i][1]* nesbat2);
					sumB += (pic2[r][i][2]* nesbat2);
					j++;
					if(nesbat-nesbat2>0){
						sum_nesbat = sum_nesbat -nesbat + nesbat2;
						nesbat2=nesbat-nesbat2;
						sum_nesbat2 += 	nesbat2;
						
					}
					else{
						nesbat2=0;
					}
				}
				if(sum_nesbat2 == nesbat){
					nesbat2=0;
					sum_nesbat2=0;
				}
			}
			pic[m][i][0] = sumR;
			pic[m][i][1] = sumG;  //sakht araye ax resize shode
			pic[m][i][2] = sumB;
			m++;
			nesbat = nesbat_h;
			sumR=0;sumG=0;sumB=0;
			sum_nesbat=0;
		}
	}
}
void DataSetAvg(int karkard){
	printf("\n\t\t\t It may take a minute please wait\n");
	int i,j,color,width,height,k,m,x;
	char path[100];
	for(i=0;i<=9;i++){
		for(j=1;j<=15;j++){
			sprintf(path,"DataSet\\%d\\%d.bmp",i,j);
			readBMP(path,&width,&height,pic);
			int h=0,w=0;
			char name1[100];
			sprintf(name1,"%d %d.bmp",i,j);
			first_crop(height,width,&h,&w);
			copy_crop_to_pic(h,w);
			double nesbat_h = 1.0* 400/h, nesbat_w = 1.0 * 300/w;
			resize_check(nesbat_w,nesbat_h , w , h,name1,karkard);
			for(k=0;k<400;k++){
				for(m=0;m<300;m++){
					for(color=0;color<3;color++){
						avg[k][m][color] += pic[k][m][color];//sum of pixel colors
					}
				}
			}
		}
		for(k=0;k<400;k++){
			for(m=0;m<300;m++){
				for(color=0;color<3;color++){
					avg[k][m][color] /= 15;//avg of pixel colors
				}
			}
		}
		if(i<=4){ //taghir shedat rang baray aadad 0-4
			x=150;
		}
		else{//taghir shedat rang baray aadad 5-9
			x=200;
		}
		for(k=0;k<400;k++){
			for(m=0;m<300;m++){
				for(color=0;color<3;color++){
					if(avg[k][m][color] < x ){
						//pic[k][m][color] = avg[k][m][color];
						pic[k][m][color] = 0;
					}
					else{
						pic[k][m][color] = 255;
					}
				}
			}
		}
		for(k=0;k<400;k++){//khali kardan arraye avg baray adad baed
			for(m=0;m<300;m++){
				for(color=0;color<3;color++){
					avg[k][m][color] = 0;
				}
			}
		}
		char name[100];
		sprintf(name,"%d.bmp",i);
		saveBMP(pic,300,400,name);
	}
	printf("\t\t\t\tDataSet Provided!\n\n");
}
void read_clock(int hour_result[]){
	char name[100];
	char path[100];
	int width,height,width1,height1,count=0,min=360000,adad,p,i,j,color,k;
	for(k=1;k<=8;){
		sprintf(name,"resize%d.bmp",k);
		readBMP(name,&width,&height,pic2);
		for(p=0;p<=9;p++){
			sprintf(path,"%d.bmp",p);//baz kardan ax avg har ragham
			readBMP(path,&width1,&height1,crop);
			for(i=0;i<400;i++){
				for(j=0;j<300;j++){
					for(color=0;color<3;color++){
						if(pic2[i][j][color] != crop[i][j][color]){
							if(abs(pic2[i][j][color]- crop[i][j][color])>235){
								/*agar ektelaf rang bishtar az 235 bood 
								1 pixel ekhtelaf dar nazar gerefte shavad*/
								count++;
							}
						}
					}
				}
			}
			if(count<min){//peyda kardan adad ba kamtarin ekhtelaf
				min = count;
				hour_result[k] = p;//zakhire clock
			}
			count = 0;
		}
		printf("%d",hour_result[k]);
		min = 360000;
		if(k+1 != 3 && k+1 != 6){//2 noghte bein saat va daghighe va saneie ra vouroodi nagirad
			k++;
		}
		else{
			printf(" : "); //chap 2 noghte
			k+=2;
		}
	}
}
