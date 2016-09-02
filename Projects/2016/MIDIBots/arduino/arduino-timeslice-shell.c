
//Timeslice global variables
unsigned long TimeSlice_TC1_Interval = 20;
unsigned long TimeSlice_TC2_Interval = 100;
unsigned long TimeSlice_TC3_Interval = 500;

unsigned long TimeSlice_TC1_Start_Time;
unsigned long TimeSlice_TC2_Start_Time;
unsigned long TimeSlice_TC3_Start_Time;



void loop()
{


//----------------------------------------------------------TimeSlice code-----------------------------------------------------------
while(1){
//-------------------
//Timeclass 1 20 ms
//-------------------
TimeSlice_TC1_Start_Time = millis();

// ---------------------------------------------------------User application code


// ---------------------------------------------------------End of user application code
// Toggle TC1 LED
toggle_LED(1);
// check timeout
if (((millis() - TimeSlice_TC1_Start_Time) > TimeSlice_TC1_Interval)) // 20 ms
{
while(1){
       over_run_Fault(1); // abort program and loop forever and flash Time class 1 LED
       }          
}
else
{
while (((millis() - TimeSlice_TC1_Start_Time) <= TimeSlice_TC1_Interval)){ // remove to save MIPS but it removes precise timeclass execution 
 }  
} 
 
//-------------------
//Timeclass 2 100 ms
//-------------------
TimeSlice_TC2_Start_Time = millis();
// ----------------------------------------------------------User application code

// ----------------------------------------------------------End of user application code
// Toggle TC2 LED
toggle_LED(2); 

if (((millis() - TimeSlice_TC2_Start_Time) > TimeSlice_TC2_Interval)) // 100 ms
{
while(1){
       over_run_Fault(2); // abort program and loop forever flash Time class 2 LED
       }        
}
else
{
while (((millis() - TimeSlice_TC2_Start_Time) <= TimeSlice_TC2_Interval)){  // remove to save MIPS but it removes precise timeclass execution 
  }  
}

//------------------
//Timeclass 3 500 ms
//------------------
TimeSlice_TC3_Start_Time = millis();
//------------------------------------------------------------------User application code


// -----------------------------------------------------------------End of user application code
// Toggle TC3 LED
toggle_LED(3);
if (((millis() - TimeSlice_TC3_Start_Time) > TimeSlice_TC3_Interval)) // 500 ms
{
while(1){
        over_run_Fault(3); // abort program loop forever flash Time class 3 LED
        }     
}
else
{  
while (((millis() - TimeSlice_TC3_Start_Time) <= TimeSlice_TC3_Interval)){  // remove to save MIPS but it removes precise timeclass execution 
 }    
}  
} // End of main while loop



}// end of main loop
