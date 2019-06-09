#include "errorstrings.gen.h"

const char * CCONV
Phidget_strerror(PhidgetReturnCode code) {

	switch (code) {
	case EPHIDGET_OK:
		return ("Success");
	case EPHIDGET_PERM:
		return ("Not Permitted");
	case EPHIDGET_NOENT:
		return ("No Such Entity");
	case EPHIDGET_TIMEOUT:
		return ("Timed Out");
	case EPHIDGET_KEEPALIVE:
		return ("Keep Alive Failure");
	case EPHIDGET_INTERRUPTED:
		return ("Op Interrupted");
	case EPHIDGET_IO:
		return ("IO Issue");
	case EPHIDGET_NOMEMORY:
		return ("Memory Issue");
	case EPHIDGET_ACCESS:
		return ("Access (Permission) Issue");
	case EPHIDGET_FAULT:
		return ("Address Issue");
	case EPHIDGET_BUSY:
		return ("Resource Busy");
	case EPHIDGET_EXIST:
		return ("Object Exists");
	case EPHIDGET_NOTDIR:
		return ("Object is not a directory");
	case EPHIDGET_ISDIR:
		return ("Object is a directory");
	case EPHIDGET_INVALID:
		return ("Invalid");
	case EPHIDGET_NFILE:
		return ("Too many open files in system");
	case EPHIDGET_MFILE:
		return ("Too many open files");
	case EPHIDGET_NOSPC:
		return ("Not enough space");
	case EPHIDGET_FBIG:
		return ("File too Big");
	case EPHIDGET_ROFS:
		return ("Read Only Filesystem");
	case EPHIDGET_RO:
		return ("Read Only Object");
	case EPHIDGET_UNSUPPORTED:
		return ("Operation Not Supported");
	case EPHIDGET_INVALIDARG:
		return ("Invalid Argument");
	case EPHIDGET_AGAIN:
		return ("Try again");
	case EPHIDGET_NOTEMPTY:
		return ("Not Empty");
	case EPHIDGET_UNEXPECTED:
		return ("Unexpected Error");
	case EPHIDGET_DUPLICATE:
		return ("Duplicate");
	case EPHIDGET_BADPASSWORD:
		return ("Bad Credential");
	case EPHIDGET_NETUNAVAIL:
		return ("Network Unavailable");
	case EPHIDGET_CONNREF:
		return ("Connection Refused");
	case EPHIDGET_CONNRESET:
		return ("Connection Reset");
	case EPHIDGET_HOSTUNREACH:
		return ("No route to host");
	case EPHIDGET_NODEV:
		return ("No Such Device");
	case EPHIDGET_WRONGDEVICE:
		return ("Wrong Device");
	case EPHIDGET_PIPE:
		return ("Broken Pipe");
	case EPHIDGET_RESOLV:
		return ("Name Resolution Failure");
	case EPHIDGET_UNKNOWNVAL:
		return ("Unknown or Invalid Value");
	case EPHIDGET_NOTATTACHED:
		return ("Device not Attached");
	case EPHIDGET_INVALIDPACKET:
		return ("Invalid or Unexpected Packet");
	case EPHIDGET_2BIG:
		return ("Argument List Too Long");
	case EPHIDGET_BADVERSION:
		return ("Bad Version");
	case EPHIDGET_CLOSED:
		return ("Closed");
	case EPHIDGET_NOTCONFIGURED:
		return ("Not Configured");
	case EPHIDGET_EOF:
		return ("End of File");
	case EPHIDGET_FAILSAFE:
		return ("Failsafe Triggered");
	default:
		return ("Invalid return code");
	}
}

const char * CCONV
Phidget_strerrordetail(PhidgetReturnCode code) {

	switch (code) {
	case EPHIDGET_OK:
		return ("Call succeeded.");
	case EPHIDGET_PERM:
		return (NULL);
	case EPHIDGET_NOENT:
		return ("The specified entity does not exist. This is usually a result of Net or Log API calls.");
	case EPHIDGET_TIMEOUT:
		return ("Call has timed out. This can happen for a number of common reasons: Check that the Phidget you are trying to open is plugged in, and that the addressing parameters have been specified correctly. Check that the Phidget is not already open in another program, such as the Phidget Control Panel, or another program you are developing. If your Phidget has a plug or terminal block for external power, ensure it is plugged in and powered. If you are using remote Phidgets, ensure that your computer can access the remote Phidgets using the Phidget Control Panel. If you are using remote Phidgets, ensure you have enabled Server Discovery or added the server corresponding to the Phidget you are trying to open. If you are using Network Server Discovery, try extending the timeout to allow more time for the server to be discovered.");
	case EPHIDGET_KEEPALIVE:
		return (NULL);
	case EPHIDGET_INTERRUPTED:
		return ("The operation was interrupted; either from an error, or because the device was closed.");
	case EPHIDGET_IO:
		return (NULL);
	case EPHIDGET_NOMEMORY:
		return (NULL);
	case EPHIDGET_ACCESS:
		return ("Access to the resource (file) is denied. This can happen when enabling logging.");
	case EPHIDGET_FAULT:
		return (NULL);
	case EPHIDGET_BUSY:
		return ("Specified resource is in use. This error code is not normally used.");
	case EPHIDGET_EXIST:
		return (NULL);
	case EPHIDGET_NOTDIR:
		return (NULL);
	case EPHIDGET_ISDIR:
		return (NULL);
	case EPHIDGET_INVALID:
		return (NULL);
	case EPHIDGET_NFILE:
		return (NULL);
	case EPHIDGET_MFILE:
		return (NULL);
	case EPHIDGET_NOSPC:
		return ("The provided buffer argument size is too small.");
	case EPHIDGET_FBIG:
		return (NULL);
	case EPHIDGET_ROFS:
		return (NULL);
	case EPHIDGET_RO:
		return (NULL);
	case EPHIDGET_UNSUPPORTED:
		return ("This API call is not supported. For Class APIs this means that this API is not supported by this device. This can also mean the API is not supported on this OS, or OS configuration.");
	case EPHIDGET_INVALIDARG:
		return ("One or more of the parameters passed to the function is not accepted by the channel in its current configuration. This may also be an indication that a NULL pointer was passed where a valid pointer is required.");
	case EPHIDGET_AGAIN:
		return (NULL);
	case EPHIDGET_NOTEMPTY:
		return (NULL);
	case EPHIDGET_UNEXPECTED:
		return ("Something unexpected has occured. Enable library logging and have a look at the log, or contact Phidgets support.");
	case EPHIDGET_DUPLICATE:
		return ("Duplicated request. Can happen with some Net API calls, such as trying to add the same server twice.");
	case EPHIDGET_BADPASSWORD:
		return (NULL);
	case EPHIDGET_NETUNAVAIL:
		return (NULL);
	case EPHIDGET_CONNREF:
		return (NULL);
	case EPHIDGET_CONNRESET:
		return (NULL);
	case EPHIDGET_HOSTUNREACH:
		return (NULL);
	case EPHIDGET_NODEV:
		return (NULL);
	case EPHIDGET_WRONGDEVICE:
		return ("A Phidget channel object of the wrong channel class was passed into this API call.");
	case EPHIDGET_PIPE:
		return (NULL);
	case EPHIDGET_RESOLV:
		return (NULL);
	case EPHIDGET_UNKNOWNVAL:
		return ("The value is unknown. This can happen right after attach, when the value has not yet been recieved from the Phidget. This can also happen if a device has not yet been configured / enabled. Some properties can only be read back after being set.");
	case EPHIDGET_NOTATTACHED:
		return ("This can happen for a number of common reasons. Be sure you are opening the channel before trying to use it. If you are opening the channel, the program may not be waiting for the channel to be attached. If possible use openWaitForAttachment. Otherwise, be sure to check the Attached property of the channel before trying to use it.");
	case EPHIDGET_INVALIDPACKET:
		return (NULL);
	case EPHIDGET_2BIG:
		return (NULL);
	case EPHIDGET_BADVERSION:
		return (NULL);
	case EPHIDGET_CLOSED:
		return ("Channel was closed. This can happen if a channel is closed while openWaitForAttachment is waiting.");
	case EPHIDGET_NOTCONFIGURED:
		return ("Device is not configured enough for this API call. Have a look at the must-set properties for this device and make sure to configure them first.");
	case EPHIDGET_EOF:
		return (NULL);
	case EPHIDGET_FAILSAFE:
		return ("Failsafe Triggered on this channel. Close and Re-open the channel to resume operation.");
	default:
		return ("Invalid return code");
	}
}
