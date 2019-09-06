
/*

  dcetest.c 
  Written by: Dave Aitel & Pasi Eronen

  This code under GPL v 2.0 .
  See gpl.txt for more information.

*/

#include "tcpstuff.h"

#define DCEPORT 135
#define BINDPACKET 0x0b
#define REQUESTPACKET 0x00
#define RESPONSEPACKET 0x02
#define FLAGS 0x03		/* PFC_FIRST_FRAG | PFC_LAST_FRAG */
#define EPT_MAX_ANNOTATION_SIZE 64
#define EPT_LOOKUP 2
#define RPC_C_EP_ALL_ELTS 0

struct uuid_t {
  unsigned int time_low;
  unsigned int time_mid;
  unsigned int time_hi_and_version;
  unsigned int clock_seq_hi_and_reserved;
  unsigned int clock_seq_low;
  unsigned char node[6];
};

/*UUID for ifids requests*/
static const struct uuid_t IFIDS_UUID = 
  {
    0xafa8bd80, 0x7d8a, 0x11c9, 0xbe, 0xf4,
    { 0x08, 0x00, 0x2b, 0x10, 0x29, 0x89 }
  };

/* UUID for Endpoint mapper interface */
static const struct uuid_t EPT_UUID =
{
  0xe1af8308, 0x5d1f, 0x11c9, 0x91, 0xa4,
  { 0x08, 0x00, 0x2b, 0x14, 0xa0, 0xfa }
};
#define EPT_VERSION 3
#define IFIDS_VERSION 1

/* UUID for DCE RPC transfer syntax */
static const struct uuid_t TRANSFER_SYNTAX_UUID =
{
  0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8,
  { 0x08, 0x00, 0x2b, 0x10, 0x48, 0x60 }
};
#define TRANSFER_SYNTAX_VERSION 2

struct lookup_handle_t {
  unsigned int attributes;
  struct uuid_t uuid;
};

static unsigned int next_call_id = 0;
static unsigned int little_endian;

void usage()
{
  printf("Usage: ./ifids host port\n");
  printf("Version 0.1 brought to you by Dave Aitel, dave@immunitysec.com.\n");
  printf("Please e-mail me if you have any questions/comments/etc\n");
  printf("Run this program at your own risk.\n");
  printf("This program under GPL v 2.0\n");
  exit(1);
}


unsigned int
load_intel_short(const unsigned char *from)
{
  return (((unsigned int) from[0]) |
	  ((unsigned int) from[1]) << 8);
}

unsigned int
load_network_short(const unsigned char *from)
{
  return (((unsigned int) from[1]) |
	  ((unsigned int) from[0]) << 8);
}

unsigned int
load_intel_long(const unsigned char *from)
{
  return (((unsigned int) from[0]) |
	  ((unsigned int) from[1]) << 8 |
	  ((unsigned int) from[2]) << 16 |
	  ((unsigned int) from[3]) << 24);
}

unsigned int
load_network_long(const unsigned char *from)
{
  return (((unsigned int) from[3]) |
	  ((unsigned int) from[2]) << 8 |
	  ((unsigned int) from[1]) << 16 |
	  ((unsigned int) from[0]) << 24);
}

unsigned int
load_short(const unsigned char *from)
{
  if (little_endian)
    return load_intel_short(from);
  else
    return load_network_short(from);
}

unsigned long
load_long(const unsigned char *from)
{
  if (little_endian)
    return load_intel_long(from);
  else
    return load_network_long(from);
}

void
store_intel_short(unsigned char *to, unsigned int value)
{
  to[0] = value & 0xff;
  to[1] = (value >> 8) & 0xff;
}

void
store_intel_long(unsigned char *to, unsigned int value)
{
  to[0] = value & 0xff;
  to[1] = (value >> 8) & 0xff;
  to[2] = (value >> 16) & 0xff;
  to[3] = (value >> 24) & 0xff;
}

void
load_uuid(const unsigned char *buffer, struct uuid_t* uuid)
{
  uuid->time_low = load_long(buffer);
  uuid->time_mid = load_short(buffer+4);
  uuid->time_hi_and_version = load_short(buffer+6);
  uuid->clock_seq_hi_and_reserved = buffer[8];
  uuid->clock_seq_low = buffer[9];
  memcpy(uuid->node, buffer+10, 6);
}

void
load_intel_uuid(const unsigned char *buffer, struct uuid_t* uuid)
{
  uuid->time_low = load_intel_long(buffer);
  uuid->time_mid = load_intel_short(buffer+4);
  uuid->time_hi_and_version = load_intel_short(buffer+6);
  uuid->clock_seq_hi_and_reserved = buffer[8];
  uuid->clock_seq_low = buffer[9];
  memcpy(uuid->node, buffer+10, 6);
}

int
store_intel_uuid(unsigned char *buffer, const struct uuid_t* uuid)
{
  store_intel_long(buffer, uuid->time_low);
  store_intel_short(buffer+4, uuid->time_mid);
  store_intel_short(buffer+6, uuid->time_hi_and_version);
  buffer[8] = uuid->clock_seq_hi_and_reserved;
  buffer[9] = uuid->clock_seq_low;
  memcpy(buffer+10, uuid->node, 6);
  return 16;
}

char *
uuid_to_string(char *buffer, const struct uuid_t* uuid)
{
  
  sprintf(buffer, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
	  uuid->time_low, uuid->time_mid, uuid->time_hi_and_version,
	  uuid->clock_seq_hi_and_reserved,
	  uuid->clock_seq_low,
	  uuid->node[0], uuid->node[1],
	  uuid->node[2], uuid->node[3],
	  uuid->node[4], uuid->node[5]);
  return buffer;
}


int 
dce_enum_bind(int fd)
{
  unsigned char buffer[1000];
  unsigned char *frag_lengthp;
  int frag_length;
  unsigned char *p;

  memset(buffer, 0x00, sizeof(buffer));
  p = buffer;

  /* Bind PDU header */
  
  *p++ = 0x05; /*version*/
  *p++ = 0x00; /*minor version*/
  *p++ = BINDPACKET; 
  *p++ = FLAGS;

  /* data representation (little endian, ASCII, IEEE floating point) */
  *p++ = 0x10;
  *p++ = 0x00;
  *p++ = 0x00;
  *p++ = 0x00;

  /* now we have 2 bytes of frag length */
  frag_lengthp = buffer+8; /*copy the length of the fragment here later*/
  p += 2;

  /* auth length = 0 */
  p += 2;

  /* call id */
  store_intel_long(p, next_call_id++);
  p += 4;

  /* max xmit frag */
  store_intel_short(p, 4096);
  p += 2;
  /* max recv frag */
  store_intel_short(p, 4096);
  p += 2;

  /* assoc group = 0 */
  p += 4;

  /* num ctx items */
  *p++ = 1;
  /* alignment padding */
  p += 3;

  /* u_int16 p_cont_id */
  *p++ = 0x00;
  *p++ = 0x00;

  /* u_int8 n_transfer_syn */
  *p++ = 1;
  /* alignment padding */
  p += 1;

  /* Interface UUID + version */
  p += store_intel_uuid(p, &IFIDS_UUID);
  store_intel_long(p, IFIDS_VERSION);
  p += 4;

  /* Transfer syntax UUID + version*/
  p += store_intel_uuid(p, &TRANSFER_SYNTAX_UUID);
  store_intel_long(p, TRANSFER_SYNTAX_VERSION);
  p += 4;
  
  /* printf("bind frag length=%d\n",p-buffer); */
  frag_length = p-buffer;
  store_intel_short(frag_lengthp, frag_length);

  write_data(fd, frag_length, buffer);
  return 1;
}

int
dce_enum_get_next(int fd, struct lookup_handle_t *entry_handle)
{
  unsigned char buffer[1000];
  unsigned char *frag_lengthp;
  int frag_length;
  unsigned char *p;

  memset(buffer, 0x00, sizeof(buffer));
  p = buffer;

  /* Start of Request PDU header */
  
  *p++ = 0x05; /*version*/
  *p++ = 0x00; /*minor version*/
  *p++ = REQUESTPACKET; 
  *p++ = FLAGS;

  /* data representation (little endian, ASCII, IEEE floating point) */
  *p++ = 0x10;
  *p++ = 0x00;
  *p++ = 0x00;
  *p++ = 0x00;

  /* now we have 2 bytes of frag length */
  frag_lengthp = buffer+8; /*copy the length of the fragment here later*/
  p += 2;

  /* auth length = 0 */
  p += 2;

  /* call id */
  store_intel_long(p, next_call_id++);
  p += 4;

  /* alloc_hint (0) */
  p += 4;
  
  /* context ID (0) */
  p += 2;

  /* opnum */
  store_intel_short(p, EPT_LOOKUP);
  p += 2;

  /* End of Request PDU header */
  
  /* Parameters for ept_lookup */

  /* unsigned32 inquiry_type */
  store_intel_long(p, RPC_C_EP_ALL_ELTS);
  p += 4;

  /* uuid_p_t object = NULL */
  p += 4;

  /* rpc_if_id_p_t interface_id = NULL */
  p += 4;

  /* unsigned32 vers_option = 0 (not used) */
  p += 4;
  
  /* unsigned32 entry_handle.attributes */
  store_intel_long(p, entry_handle->attributes);
  p += 4;
  
  /* uuid_t entry_handle.context_handle_uuid */
  p += store_intel_uuid(p, &entry_handle->uuid);

  /* unsigned32 max_ents */
  store_intel_long(p, 1);
  p += 4;
  
  frag_length = p-buffer;
  store_intel_short(frag_lengthp, frag_length);

  write_data(fd, frag_length, buffer);
  return 1;
}

int
read_dce_pdu(int fd, unsigned char *buf)
{
  int frag_length;
  read_data(fd, 16, buf);
  if ((buf[4] & 0xf0) == 0x10) {
    little_endian = 1;
  } else {
    little_endian = 0;
  }
  frag_length = load_short(buf+8);
  read_data(fd, frag_length-16, buf+16);
  return frag_length;
}

/*ask for what interfaces are listening on this port*/
void
dce_ifids_request(int fd)
{

  unsigned char buffer[1000];
  unsigned char *frag_lengthp;
  int frag_length;
  unsigned char *p;

  memset(buffer, 0x00, sizeof(buffer));
  p = buffer;
  /* Start of Request PDU header */
  
  *p++ = 0x05; /*version*/
  *p++ = 0x00; /*minor version*/
  *p++ = REQUESTPACKET; 
  *p++ = FLAGS;


  /* data representation (little endian, ASCII, IEEE floating point) */
  *p++ = 0x10;
  *p++ = 0x00;
  *p++ = 0x00;
  *p++ = 0x00;

  /* now we have 2 bytes of frag length */
  frag_lengthp = buffer+8; /*copy the length of the fragment here later*/
  p += 2;

  /* auth length = 0 */
  p += 2;

  /* call id */
  store_intel_long(p, next_call_id++);
  p += 4;

  /* alloc_hint (0) */
  p += 4;
  
  /* context ID (0) */
  p += 2;

  /* opnum (0)*/
  store_intel_short(p, 0);
  p += 2;
  frag_length = p-buffer;
  store_intel_short(frag_lengthp, frag_length);

  write_data(fd, frag_length, buffer);
  return ;
}

int
dce_parse_ifids(const unsigned char *buf)
{
  const unsigned char *p;
  unsigned long howmany;
  struct uuid_t uuid;
  unsigned short versionmajor,versionminor;
  unsigned char buffer[500];
  int i;


  p = buf;
  
  if (p[2] != RESPONSEPACKET) {
    printf("Unexpected DCE PDU type %02x\n", p[2]);
    return 0;
  }
  /* Skip common DCE header */
  p += 16;

  /* Skip alloc_hint, p_cont_id, cancel_count, and padding */
  p += 8; 
  
  /*skip a word we don't understand*/
  p += 4;
  
  
  howmany=load_long(p);
  printf("Found %u entries\n",howmany);
  p+=4;

  p+=4; /*skip it again as it is repeated*/

  /*skip a bunch of stuff we don't understand - 1 for each entry*/
  p+=4*howmany;

  /*now parse it*/
  for (i=0; i< howmany; i++)
    {
      load_uuid(p, &uuid);
      p+=16;
      versionmajor=load_short(p);
      p+=2;
      versionminor=load_short(p);
      p+=2;

      memset(buffer,0x00,sizeof(buffer));
      printf("%s v%d.%d\n",uuid_to_string(buffer,&uuid),versionmajor,versionminor);
    }
  
  return 0;
}



int
dce_parse_enum_response(const unsigned char *buf,
			struct lookup_handle_t *entry_handle)
{
  const unsigned char *p;
  int tint;
  char annotation[EPT_MAX_ANNOTATION_SIZE+1];
  int floor, floors;
  int address_type;

  char tmp_address[200];
  char tmp_address2[200];

  p = buf;

  if (p[2] != RESPONSEPACKET) {
    printf("Unexpected DCE PDU type %02x\n", p[2]);
    return 0;
  }
 
  /* Skip common DCE header */
  p += 16;

  /* Skip alloc_hint, p_cont_id, cancel_count, and padding */
  p += 8; 

  /* context_handle_attributes */
  entry_handle->attributes = load_long(p);
  p += 4;

  /* context_handle_uuid */
  load_uuid(p, &entry_handle->uuid);
  p += 16;

  /* num_ents */
  if (load_long(p) == 0) {
    return 0;
  }
  p += 4;
 
  /* skip something */
  p += 36;

  /* annotation */
  tint = load_long(p);
  p += 4;
  if (tint > EPT_MAX_ANNOTATION_SIZE) {
    printf("Annotation length (%d) too long\n", tint);
    return 0;
  }
  /* annotation length includes trailing NULL */
  memcpy(annotation, p, tint);
  p += tint;
  printf("\nannotation=%s\n",annotation);
  
  while ((p-buf) % 4 != 0) {
    p++;
  }

  /* Skip tower lengths */
  p += 8;
 
  floors = load_intel_short(p);
  p += 2;

  /* printf("floors = %d\n", floors);*/

  tmp_address[0] = '\0';
  for (floor = 1; floor <= floors; floor++) {
    tint = load_intel_short(p);
    p += 2;

    /*printf("floor %d lhs = %d bytes\n", floor, tint);*/

    if (floor == 1) {
      struct uuid_t uuid;
      load_intel_uuid(p+1, &uuid);
      uuid_to_string(tmp_address, &uuid);
      printf("uuid=%s , version=%d\n", tmp_address, load_intel_short(p+17));
    } else if (floor == 2 || floor == 3) {
      /* don't print */
    } else {
      address_type = *p;
    }
    p += tint;

    tint = load_intel_short(p);
    p += 2;
    if (floor > 3) {
      switch (address_type) {
	case 0x07:
	  sprintf(tmp_address, "ncacn_ip_tcp:%%s[%d]", load_network_short(p));
	  break;
	case 0x08:
	  sprintf(tmp_address, "ncadg_ip_udp:%%s[%d]", load_network_short(p));
	  break;
	case 0x09:
	  sprintf(tmp_address2, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	  if (tmp_address[0] != '\0') {
	    printf(tmp_address, tmp_address2);
	    puts("");
	  } else {
	    printf("IP: %s\n", tmp_address2);
	  }
	  break;
	case 0x0f:
	  sprintf(tmp_address, "ncacn_np:%%s[%s]", p);
	  break;
	case 0x10:
	  printf("ncalrpc[%s]\n", p);
	  break;
	case 0x11:
	  if (tmp_address[0] != '\0') {
	    printf(tmp_address, p);
	    puts("");
	  } else {
	    printf("NetBIOS: %s\n", p);
	  }
	  break;
	case 0x1f:
	  sprintf(tmp_address, "ncacn_http:%%s[%d]", load_network_short(p));
	  break;
	default:
	  printf("unknown address type 0x%02x (floor %d, %d bytes)\n",
		 address_type, floor, tint);
      }
    }
    p += tint;
  }
  
  if ((entry_handle->uuid.time_low == 0) &&
      (entry_handle->uuid.time_mid == 0) &&
      (entry_handle->uuid.time_hi_and_version == 0) &&
      (entry_handle->uuid.clock_seq_hi_and_reserved == 0) &&
      (entry_handle->uuid.clock_seq_low == 0) &&
      memcmp(entry_handle->uuid.node, "\0\0\0\0\0\0", 6) == 0) {
    return 0;
  }
  
  return 1;
}

int
main(int argc, char *argv[])
{
  int fd;
  int morestuff = 1;
  char buffer[5000];
  struct lookup_handle_t entry_handle;

  printf("DCE-RPC IFIDS by Dave Aitel.\n");
  printf("Finds all the interfaces and versions listening on that TCP port\n");
  if (argc<3)
       usage();
  
  fd = tcpconnect(argv[1], atoi(argv[2]));
  if (fd == -1)
  {
    printf("Was not able to connect to %s:%d\n", argv[1],atoi(argv[2]));
    exit(1);
  }

  printf("Tcp Connected\n");

  dce_enum_bind(fd);
  read_dce_pdu(fd, buffer);

  memset(&entry_handle, 0, sizeof(entry_handle));
  while (morestuff)
  {
    dce_ifids_request(fd);
    read_dce_pdu(fd, buffer);
    morestuff = dce_parse_ifids(buffer);
  }

  printf("\nDone\n");
  return 1;
}

