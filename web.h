
// /*********  FUNCTIONS  **********/

// //Download a file from the SD, it is called in void SD_dir()
// void SD_file_download(String filename)
// {
//   if (SD_present) 
//   { 
//     File download = SD.open("/"+filename);
//     if (download) 
//     {
//       server.sendHeader("Content-Type", "text/text");
//       server.sendHeader("Content-Disposition", "attachment; filename="+filename);
//       server.sendHeader("Connection", "close");
//       server.streamFile(download, "application/octet-stream");
//       download.close();
//     } else ReportFileNotPresent("download"); 
//   } else ReportSDNotPresent();
// }
// //Upload a file to the SD
// void File_Upload()
// {
//   append_page_header();
//   webpage += F("<h3>Select File to Upload</h3>"); 
//   webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
//   webpage += F("<input class='buttons' style='width:25%' type='file' name='fupload' id = 'fupload' value=''>");
//   webpage += F("<button class='buttons' style='width:10%' type='submit'>Upload File</button><br><br>");
//   webpage += F("<a href='/'>[Back]</a><br><br>");
//   append_page_footer();
//   server.send(200, "text/html",webpage);
// }

// //Handles the file upload a file to the SD
// File UploadFile;
// //Upload a new file to the Filing system
// void handleFileUpload()
// { 
//   HTTPUpload& uploadfile = server.upload(); //See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
//                                             //For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
//   if(uploadfile.status == UPLOAD_FILE_START)
//   {
//     String filename = uploadfile.filename;
//     if(!filename.startsWith("/")) filename = "/"+filename;
//     Serial.print("Upload File Name: "); Serial.println(filename);
//     SD.remove(filename);                         //Remove a previous version, otherwise data is appended the file again
//     UploadFile = SD.open(filename, FILE_WRITE);  //Open the file for writing in SD (create it, if doesn't exist)
//     filename = String();
//   }
//   else if (uploadfile.status == UPLOAD_FILE_WRITE)
//   {
//     if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
//   } 
//   else if (uploadfile.status == UPLOAD_FILE_END)
//   {
//     if(UploadFile)          //If the file was successfully created
//     {                                    
//       UploadFile.close();   //Close the file again
//       Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
//       webpage = "";
//       append_page_header();
//       webpage += F("<h3>File was successfully uploaded</h3>"); 
//       webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>";
//       webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br><br>"; 
//       webpage += F("<a href='/'>[Back]</a><br><br>");
//       append_page_footer();
//       server.send(200,"text/html",webpage);
//     } 
//     else
//     {
//       ReportCouldNotCreateFile("upload");
//     }
//   }
// }

// //Initial page of the server web, list directory and give you the chance of deleting and uploading
// void SD_dir()
// {
//   if (SD_present) 
//   {
//     //Action acording to post, dowload or delete, by MC 2022
//     if (server.args() > 0 ) //Arguments were received, ignored if there are not arguments
//     { 
//       Serial.println(server.arg(0));
  
//       String Order = server.arg(0);
//       Serial.println(Order);
      
//       if (Order.indexOf("download_")>=0)
//       {
//         Order.remove(0,9);
//         SD_file_download(Order);
//         Serial.println(Order);
//       }
  
//       if ((server.arg(0)).indexOf("delete_")>=0)
//       {
//         Order.remove(0,7);
//         SD_file_delete(Order);
//         Serial.println(Order);
//       }
//     }

//     File root = SD.open("/");
//     if (root) {
//       root.rewindDirectory();
//       SendHTML_Header();    
//       webpage += F("<table align='center'>");
//       webpage += F("<tr><th>Name/Type</th><th style='width:20%'>Type File/Dir</th><th>File Size</th></tr>");
//       printDirectory("/",0);
//       webpage += F("</table>");
//       SendHTML_Content();
//       root.close();
//     }
//     else 
//     {
//       SendHTML_Header();
//       webpage += F("<h3>No Files Found</h3>");
//     }
//     append_page_footer();
//     SendHTML_Content();
//     SendHTML_Stop();   //Stop is needed because no content length was sent
//   } else ReportSDNotPresent();
// }

// //Prints the directory, it is called in void SD_dir() 
// void printDirectory(const char * dirname, uint8_t levels)
// {
  
//   File root = SD.open(dirname);

//   if(!root){
//     return;
//   }
//   if(!root.isDirectory()){
//     return;
//   }
//   File file = root.openNextFile();

//   int i = 0;
//   while(file){
//     if (webpage.length() > 1000) {
//       SendHTML_Content();
//     }
//     if(file.isDirectory()){
//       webpage += "<tr><td>"+String(file.isDirectory()?"Dir":"File")+"</td><td>"+String(file.name())+"</td><td></td></tr>";
//       printDirectory(file.name(), levels-1);
//     }
//     else
//     {
//       webpage += "<tr><td>"+String(file.name())+"</td>";
//       webpage += "<td>"+String(file.isDirectory()?"Dir":"File")+"</td>";
//       int bytes = file.size();
//       String fsize = "";
//       if (bytes < 1024)                     fsize = String(bytes)+" B";
//       else if(bytes < (1024 * 1024))        fsize = String(bytes/1024.0,3)+" KB";
//       else if(bytes < (1024 * 1024 * 1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
//       else                                  fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
//       webpage += "<td>"+fsize+"</td>";
//       webpage += "<td>";
//       webpage += F("<FORM action='/' method='post'>"); 
//       webpage += F("<button type='submit' name='download'"); 
//       webpage += F("' value='"); webpage +="download_"+String(file.name()); webpage +=F("'>Download</button>");
//       webpage += "</td>";
//       webpage += "<td>";
//       webpage += F("<FORM action='/' method='post'>"); 
//       webpage += F("<button type='submit' name='delete'"); 
//       webpage += F("' value='"); webpage +="delete_"+String(file.name()); webpage +=F("'>Delete</button>");
//       webpage += "</td>";
//       webpage += "</tr>";

//     }
//     file = root.openNextFile();
//     i++;
//   }
//   file.close();

 
// }

// //Delete a file from the SD, it is called in void SD_dir()
// void SD_file_delete(String filename) 
// { 
//   if (SD_present) { 
//     SendHTML_Header();
//     File dataFile = SD.open("/"+filename, FILE_READ); //Now read data from SD Card 
//     if (dataFile)
//     {
//       if (SD.remove("/"+filename)) {
//         Serial.println(F("File deleted successfully"));
//         webpage += "<h3>File '"+filename+"' has been erased</h3>"; 
//         webpage += F("<a href='/'>[Back]</a><br><br>");
//       }
//       else
//       { 
//         webpage += F("<h3>File was not deleted - error</h3>");
//         webpage += F("<a href='/'>[Back]</a><br><br>");
//       }
//     } else ReportFileNotPresent("delete");
//     append_page_footer(); 
//     SendHTML_Content();
//     SendHTML_Stop();
//   } else ReportSDNotPresent();
// } 

// //SendHTML_Header
// void SendHTML_Header()
// {
//   server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
//   server.sendHeader("Pragma", "no-cache"); 
//   server.sendHeader("Expires", "-1"); 
//   server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
//   server.send(200, "text/html", ""); //Empty content inhibits Content-length header so we have to close the socket ourselves. 
//   append_page_header();
//   server.sendContent(webpage);
//   webpage = "";
// }

// //SendHTML_Content
// void SendHTML_Content()
// {
//   server.sendContent(webpage);
//   webpage = "";
// }

// //SendHTML_Stop
// void SendHTML_Stop()
// {
//   server.sendContent("");
//   server.client().stop(); //Stop is needed because no content length was sent
// }

// /* //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void SelectInput(String heading1, String command, String arg_calling_name)
// {
//   SendHTML_Header();
//   webpage += F("<h3>"); webpage += heading1 + "</h3>"; 
//   webpage += F("<FORM action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
//   webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
//   webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br><br>");
//   append_page_footer();
//   SendHTML_Content();
//   SendHTML_Stop();
// } */

// //ReportSDNotPresent
// void ReportSDNotPresent()
// {
//   SendHTML_Header();
//   webpage += F("<h3>No SD Card present</h3>"); 
//   webpage += F("<a href='/'>[Back]</a><br><br>");
//   append_page_footer();
//   SendHTML_Content();
//   SendHTML_Stop();
// }

// //ReportFileNotPresent
// void ReportFileNotPresent(String target)
// {
//   SendHTML_Header();
//   webpage += F("<h3>File does not exist</h3>"); 
//   webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
//   append_page_footer();
//   SendHTML_Content();
//   SendHTML_Stop();
// }

// //ReportCouldNotCreateFile
// void ReportCouldNotCreateFile(String target)
// {
//   SendHTML_Header();
//   webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
//   webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
//   append_page_footer();
//   SendHTML_Content();
//   SendHTML_Stop();
// }

// //File size conversion
// String file_size(int bytes)
// {
//   String fsize = "";
//   if (bytes < 1024)                 fsize = String(bytes)+" B";
//   else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
//   else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
//   else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
//   return fsize;
// }

// void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
//     Serial.printf("Listing directory: %s\n", dirname);

//     File root = fs.open(dirname);
//     if(!root){
//         Serial.println("Failed to open directory");
//         return;
//     }
//     if(!root.isDirectory()){
//         Serial.println("Not a directory");
//         return;
//     }

//     File file = root.openNextFile();
//     while(file){
//         if(file.isDirectory()){
//             Serial.print("  DIR : ");
//             Serial.println(file.name());
//             if(levels){
//                 listDir(fs, file.path(), levels -1);
//             }
//         } else {
//             Serial.print("  FILE: ");
//             Serial.print(file.name());
//             Serial.print("  SIZE: ");
//             Serial.println(file.size());
//         }
//         file = root.openNextFile();
//     }
// }

// void createDir(fs::FS &fs, const char * path){
//     Serial.printf("Creating Dir: %s\n", path);
//     if(fs.mkdir(path)){
//         Serial.println("Dir created");
//     } else {
//         Serial.println("mkdir failed");
//     }
// }

// void removeDir(fs::FS &fs, const char * path){
//     Serial.printf("Removing Dir: %s\n", path);
//     if(fs.rmdir(path)){
//         Serial.println("Dir removed");
//     } else {
//         Serial.println("rmdir failed");
//     }
// }

// void readFile(fs::FS &fs, const char * path){
//     Serial.printf("Reading file: %s\n", path);

//     File file = fs.open(path);
//     if(!file){
//         Serial.println("Failed to open file for reading");
//         return;
//     }

//     Serial.print("Read from file: ");
//     while(file.available()){
//         Serial.write(file.read());
//     }
//     file.close();
// }

// void writeFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Writing file: %s\n", path);

//     File file = fs.open(path, FILE_WRITE);
//     if(!file){
//         Serial.println("Failed to open file for writing");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("File written");
//     } else {
//         Serial.println("Write failed");
//     }
//     file.close();
// }

// void appendFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Appending to file: %s\n", path);

//     File file = fs.open(path, FILE_APPEND);
//     if(!file){
//         Serial.println("Failed to open file for appending");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("Message appended");
//     } else {
//         Serial.println("Append failed");
//     }
//     file.close();
// }

// void renameFile(fs::FS &fs, const char * path1, const char * path2){
//     Serial.printf("Renaming file %s to %s\n", path1, path2);
//     if (fs.rename(path1, path2)) {
//         Serial.println("File renamed");
//     } else {
//         Serial.println("Rename failed");
//     }
// }

// void deleteFile(fs::FS &fs, const char * path){
//     Serial.printf("Deleting file: %s\n", path);
//     if(fs.remove(path)){
//         Serial.println("File deleted");
//     } else {
//         Serial.println("Delete failed");
//     }
// }

// void testFileIO(fs::FS &fs, const char * path){
//     File file = fs.open(path);
//     static uint8_t buf[512];
//     size_t len = 0;
//     uint32_t start = millis();
//     uint32_t end = start;
//     if(file){
//         len = file.size();
//         size_t flen = len;
//         start = millis();
//         while(len){
//             size_t toRead = len;
//             if(toRead > 512){
//                 toRead = 512;
//             }
//             file.read(buf, toRead);
//             len -= toRead;
//         }
//         end = millis() - start;
//         Serial.printf("%u bytes read for %u ms\n", flen, end);
//         file.close();
//     } else {
//         Serial.println("Failed to open file for reading");
//     }


//     file = fs.open(path, FILE_WRITE);
//     if(!file){
//         Serial.println("Failed to open file for writing");
//         return;
//     }

//     size_t i;
//     start = millis();
//     for(i=0; i<2048; i++){
//         file.write(buf, 512);
//     }
//     end = millis() - start;
//     Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
//     file.close();
// }


// void sd_setup(){
//   if(!SD.begin()){
//       Serial.println("Card Mount Failed");
//       return;
//   }
//   uint8_t cardType = SD.cardType();

//   if(cardType == CARD_NONE){
//       Serial.println("No SD card attached");
//       return;
//   }

//   Serial.print("SD Card Type: ");
//   if(cardType == CARD_MMC){
//       Serial.println("MMC");
//   } else if(cardType == CARD_SD){
//       Serial.println("SDSC");
//   } else if(cardType == CARD_SDHC){
//       Serial.println("SDHC");
//   } else {
//       Serial.println("UNKNOWN");
//   }

//   uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//   Serial.printf("SD Card Size: %lluMB\n", cardSize);

//   // listDir(SD, "/", 0);
//   // createDir(SD, "/mydir");
//   // listDir(SD, "/", 0);
//   // removeDir(SD, "/mydir");
//   // listDir(SD, "/", 2);
//   // writeFile(SD, "/hello.txt", "Hello ");
//   // appendFile(SD, "/hello.txt", "World!\n");
//   // readFile(SD, "/hello.txt");
//   // deleteFile(SD, "/foo.txt");
//   // renameFile(SD, "/hello.txt", "/foo.txt");
//   // readFile(SD, "/foo.txt");
//   // testFileIO(SD, "/test.txt");
//   // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
//   // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
// }

