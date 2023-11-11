#include <stdio.h>
#include <curl/curl.h>



size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{

	return fwrite(ptr, size, nmemb, (FILE *) userdata);

}


int call_curl(char* hash)
{
  CURL *curl;
  CURLcode res;
  FILE * f = NULL;

  curl = curl_easy_init();
  if(curl) {



    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.virustotal.com/api/v3/files/"+hash);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    headers = curl_slist_append(headers, "x:apikey: "71eafebbcd8e82c002e4b527ae523f3fabcbf129058d8b5b38499840f1e5aa4c"");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    f = fopen("tmp.txt", "w+");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}
