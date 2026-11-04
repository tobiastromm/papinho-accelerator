static int personal_console_write(HANDLE h,const char *s)
{DWORD n,w;size_t z=strlen(s);if(z>MAXDWORD)return 0;n=(DWORD)z;return WriteConsoleA(h,s,n,&w,NULL)&&w==n;}
/* Same console transport for nonsecret candidate DATA and exact confirmation.
 * Reading DATA does not attribute an act, authenticate a peer or consume it. */
static int personal_console_line(HANDLE in,HANDLE out,char *reply,size_t capacity)
{
 INPUT_RECORD event;DWORD count;char text[2];size_t used=0;
 DWORD wait;unsigned repeat;unsigned char ch;
 if(!reply||capacity<2U)return 0;reply[0]=0;
 /* ReadConsoleInput does not echo: explicitly display only this nonsecret
  * ASCII declaration. Never change shared console modes or log input. */
 if(!personal_console_write(out,"> "))return 0;
 for(;;){
 if(papacc_server_console_win32_stop_requested())return 0;
 wait=WaitForSingleObject(in,50U);if(wait==WAIT_TIMEOUT)continue;if(wait!=WAIT_OBJECT_0)return 0;
 if(!ReadConsoleInputA(in,&event,1U,&count)||count!=1U)return 0;
 if(event.EventType!=KEY_EVENT||!event.Event.KeyEvent.bKeyDown)continue;
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE){(void)personal_console_write(out,"\r\nCONFIRMATION_CANCELLED\r\n");return 0;}
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_RETURN){
  if(!personal_console_write(out,"\r\n"))return 0;
  reply[used]=0;return 1;
 }
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_BACK){
  for(repeat=0;repeat<event.Event.KeyEvent.wRepeatCount&&used;repeat++){
   --used;if(!personal_console_write(out,"\b \b"))return 0;
  }
  continue;
 }
 ch=(unsigned char)event.Event.KeyEvent.uChar.AsciiChar;
 if(ch<32U||ch>126U)continue;
 text[0]=(char)ch;text[1]=0;
 for(repeat=0;repeat<event.Event.KeyEvent.wRepeatCount;repeat++){
  if(used>=capacity-1U){(void)personal_console_write(out,"\r\nCONFIRMATION_TOO_LONG_DENY\r\n");return 0;}
  reply[used++]=(char)ch;if(!personal_console_write(out,text))return 0;
 }
 }
}
static int personal_console_confirm(HANDLE in,HANDLE out,const char *expected)
{char reply[128];return personal_console_line(in,out,reply,sizeof(reply))&&strcmp(reply,expected)==0;}
