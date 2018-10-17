// ************************************ JSON *************************************** //

uint8_t json_find(char * str, const char * marker, char * dst, uint16_t maxlen)
{
  str = strstr(str, marker);

  if (str == NULL) return 0;

  str += strlen(marker) + 2;// wskaÄ‚â€žĂ„â€¦Ă„Ä…ÄąĹźnik na poczĂ„â€šĂ˘â‚¬ĹľÄ‚Ë�Ă˘â€šÂ¬Ă‚Â¦tek wartoÄ‚â€žĂ„â€¦Ä‚Ë�Ă˘â€šÂ¬ÄąĹşci, 2 = ":

  char end_delim;

  if (*str == '\"')
  {
    end_delim = '\"';
    str++;
  }
  else end_delim = ',';

  char * end = strchr(str, end_delim);

  if (end == NULL || end - str > maxlen) return 0;

  memcpy(dst, str, end - str);
  dst[end - str] = 0;

  return 1;
}

uint8_t is_json(char * str)
{
  str = strchr(str, '{');
  if (str == NULL) return 0;
  str++;

  str = strchr(str, '\"');
  if (str == NULL) return 0;
  str++;

  str = strstr(str, "\":");

  return (str != NULL);
}

// ************************************ XML *************************************** //

uint8_t xml_find(char * str, const char * marker, char * dst, uint16_t maxlen)
{
  str = strstr(str, marker);

  if (str == NULL) return 0;

  uint8_t len = strlen(marker);
  str += len + (marker[len-1]!='>'); // wskaÄ‚â€žĂ„â€¦Ă„Ä…ÄąĹźnik na poczĂ„â€šĂ˘â‚¬ĹľÄ‚Ë�Ă˘â€šÂ¬Ă‚Â¦tek wartoÄ‚â€žĂ„â€¦Ä‚Ë�Ă˘â€šÂ¬ÄąĹşci, +1 jeÄ‚â€žĂ„â€¦Ä‚Ë�Ă˘â€šÂ¬ÄąĹşli to tylko zawartoÄ‚â€žĂ„â€¦Ä‚Ë�Ă˘â€šÂ¬ÄąĹşĂ„â€šĂ˘â‚¬ĹľÄ‚Ë�Ă˘â€šÂ¬Ă‹â€ˇ znacznika

  char end_delim;

  if (*str == '<') // jest CDATA
  {
    end_delim = ']';

    str = strchr(strchr(str, '[') + 1, '[') + 1;
  }
  else end_delim = '<';

  char * end = strchr(str, end_delim);

  if (end == NULL || end - str > maxlen) return 0;

  memcpy(dst, str, end - str);
  dst[end - str] = 0;

  return 1;
}

uint8_t is_xml(char * str)
{
  str = strchr(str, '<');

  if (str == NULL) return 0;
  str++;

  str = strchr(str, '>');

  return (str != NULL);
}





