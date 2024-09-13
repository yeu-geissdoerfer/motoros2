//ServiceReadWriteVariables.h

// SPDX-FileCopyrightText: 2024, Yaskawa Europe, GmbH
// SPDX-FileCopyrightText: 2024, Delft University of Technology
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MOTOROS2_SERVICE_READ_WRITE_VARIABLE_H
#define MOTOROS2_SERVICE_READ_WRITE_VARIABLE_H

extern rcl_service_t g_serviceReadVarByte;
extern rcl_service_t g_serviceReadVarDouble;
extern rcl_service_t g_serviceReadVarInteger;
extern rcl_service_t g_serviceReadVarPosition;
extern rcl_service_t g_serviceReadVarReal;
extern rcl_service_t g_serviceReadVarString;

extern rcl_service_t g_serviceWriteVarByte;
extern rcl_service_t g_serviceWriteVarDouble;
extern rcl_service_t g_serviceWriteVarInteger;
extern rcl_service_t g_serviceWriteVarPosition;
extern rcl_service_t g_serviceWriteVarReal;
extern rcl_service_t g_serviceWriteVarString;

typedef struct
{
    motoros2_interfaces__srv__ReadVarByte_Request req_byte_var_read;
    motoros2_interfaces__srv__ReadVarByte_Response resp_byte_var_read;

    motoros2_interfaces__srv__ReadVarDouble_Request req_double_var_read;
    motoros2_interfaces__srv__ReadVarDouble_Response resp_double_var_read;

    motoros2_interfaces__srv__ReadVarInteger_Request req_int_var_read;
    motoros2_interfaces__srv__ReadVarInteger_Response resp_int_var_read;

    motoros2_interfaces__srv__ReadVarPosition_Request req_pos_var_read;
    motoros2_interfaces__srv__ReadVarPosition_Response resp_pos_var_read;

    motoros2_interfaces__srv__ReadVarReal_Request req_real_var_read;
    motoros2_interfaces__srv__ReadVarReal_Response resp_real_var_read;

    motoros2_interfaces__srv__ReadVarString_Request req_str_var_read;
    motoros2_interfaces__srv__ReadVarString_Response resp_str_var_read;


    motoros2_interfaces__srv__WriteVarByte_Request req_byte_var_write;
    motoros2_interfaces__srv__WriteVarByte_Response resp_byte_var_write;

    motoros2_interfaces__srv__WriteVarDouble_Request req_double_var_write;
    motoros2_interfaces__srv__WriteVarDouble_Response resp_double_var_write;

    motoros2_interfaces__srv__WriteVarInteger_Request req_int_var_write;
    motoros2_interfaces__srv__WriteVarInteger_Response resp_int_var_write;

    motoros2_interfaces__srv__WriteVarPosition_Request req_pos_var_write;
    motoros2_interfaces__srv__WriteVarPosition_Response resp_pos_var_write;

    motoros2_interfaces__srv__WriteVarReal_Request req_real_var_write;
    motoros2_interfaces__srv__WriteVarReal_Response resp_real_var_write;

    motoros2_interfaces__srv__WriteVarString_Request req_str_var_write;
    motoros2_interfaces__srv__WriteVarString_Response resp_str_var_write;

} ServiceReadWriteVariable_Messages;

extern ServiceReadWriteVariable_Messages g_messages_ReadWriteVariable;


void Ros_ServiceReadWriteVariable_Initialize();
void Ros_ServiceReadWriteVariable_Cleanup();

void Ros_ServiceReadVarByte_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceReadVarDouble_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceReadVarInteger_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceReadVarPosition_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceReadVarReal_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceReadVarString_Trigger(const void* request_msg, void* response_msg);

void Ros_ServiceWriteVarByte_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceWriteVarDouble_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceWriteVarInteger_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceWriteVarPosition_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceWriteVarReal_Trigger(const void* request_msg, void* response_msg);
void Ros_ServiceWriteVarString_Trigger(const void* request_msg, void* response_msg);

#endif // MOTOROS2_SERVICE_READ_WRITE_VARIABLE_H
