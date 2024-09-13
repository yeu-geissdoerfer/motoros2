//ServiceReadWriteVariables.c

// SPDX-FileCopyrightText: 2024, Yaskawa Europe, GmbH
// SPDX-FileCopyrightText: 2024, Delft University of Technology
//
// SPDX-License-Identifier: Apache-2.0

#include "MotoROS.h"

typedef enum
{
    // TODO(gavanderhoorn): not too nice, but at least we shorten the names a little
    // TODO(gavanderhoorn): generalise the result codes more, so they are not Yaskawa-specific any more
    
    VAR_RESULT_OK = motoros2_interfaces__msg__VarResultCodes__OK,
    VAR_RESULT_READ_API_ERROR = motoros2_interfaces__msg__VarResultCodes__READ_API_ERROR,
    VAR_RESULT_WRITE_API_ERROR = motoros2_interfaces__msg__VarResultCodes__WRITE_API_ERROR,
    VAR_RESULT_READ_VAR_NUMBER_INVALID = motoros2_interfaces__msg__VarResultCodes__READ_VAR_NUMBER_INVALID,
    VAR_RESULT_WRITE_VAR_NUMBER_INVALID = motoros2_interfaces__msg__VarResultCodes__WRITE_VAR_NUMBER_INVALID


    /*
    IO_RESULT_READ_ADDRESS_INVALID = motoros2_interfaces__msg__IoResultCodes__READ_ADDRESS_INVALID,
    IO_RESULT_WRITE_ADDRESS_INVALID = motoros2_interfaces__msg__IoResultCodes__WRITE_ADDRESS_INVALID,
    IO_RESULT_WRITE_VALUE_INVALID = motoros2_interfaces__msg__IoResultCodes__WRITE_VALUE_INVALID,
    IO_RESULT_READ_API_ERROR = motoros2_interfaces__msg__IoResultCodes__READ_API_ERROR,
    IO_RESULT_WRITE_API_ERROR = motoros2_interfaces__msg__IoResultCodes__WRITE_API_ERROR
    */

    // ToDo(Jonas): Use error messages from MotoPlus, but not all will make sense!


} VariableResultCodes;


static BOOL Ros_VarServer_IsValidVarNumber(UINT32 var_number);
static const char* const Ros_VarServer_ResultCodeToStr(UINT32 resultCode);


rcl_service_t g_serviceReadVarByte;
rcl_service_t g_serviceReadVarDouble;
rcl_service_t g_serviceReadVarInteger;
rcl_service_t g_serviceReadVarPosition;
rcl_service_t g_serviceReadVarReal;
rcl_service_t g_serviceReadVarString;

rcl_service_t g_serviceWriteVarByte;
rcl_service_t g_serviceWriteVarDouble;
rcl_service_t g_serviceWriteVarInteger;
rcl_service_t g_serviceWriteVarPosition;
rcl_service_t g_serviceWriteVarReal;
rcl_service_t g_serviceWriteVarString;

ServiceReadWriteVariable_Messages g_messages_ReadWriteVariable;


void Ros_ServiceReadWriteVariable_Initialize()
{
    MOTOROS2_MEM_TRACE_START(svc_rw_var_init);

    rcl_ret_t ret;

    const rosidl_service_type_support_t* t_supp_read_var_byte = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarByte);
    const rosidl_service_type_support_t* t_supp_read_var_double = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarDouble);
    const rosidl_service_type_support_t* t_supp_read_var_integer = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarInteger);
    const rosidl_service_type_support_t* t_supp_read_var_position = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarPosition);
    const rosidl_service_type_support_t* t_supp_read_var_real = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarReal);
    const rosidl_service_type_support_t* t_supp_read_var_string = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, ReadVarString);

    const rosidl_service_type_support_t* t_supp_write_var_byte = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarByte);
    const rosidl_service_type_support_t* t_supp_write_var_double = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarDouble);
    const rosidl_service_type_support_t* t_supp_write_var_integer = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarInteger);
    const rosidl_service_type_support_t* t_supp_write_var_position = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarPosition);
    const rosidl_service_type_support_t* t_supp_write_var_real = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarReal);
    const rosidl_service_type_support_t* t_supp_write_var_string = ROSIDL_GET_SRV_TYPE_SUPPORT(motoros2_interfaces, srv, WriteVarString);


    ret = rclc_service_init_default(&g_serviceReadVarByte, &g_microRosNodeInfo.node, t_supp_read_var_byte, SERVICE_NAME_READ_VAR_BYTE);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_BYTE, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceReadVarDouble, &g_microRosNodeInfo.node, t_supp_read_var_double, SERVICE_NAME_READ_VAR_DOUBLE);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_DOUBLE, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceReadVarInteger, &g_microRosNodeInfo.node, t_supp_read_var_integer, SERVICE_NAME_READ_VAR_INTEGER);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_INTEGER, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceReadVarPosition, &g_microRosNodeInfo.node, t_supp_read_var_position, SERVICE_NAME_READ_VAR_POSITION);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_POSITION, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceReadVarReal, &g_microRosNodeInfo.node, t_supp_read_var_real, SERVICE_NAME_READ_VAR_REAL);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_REAL, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceReadVarString, &g_microRosNodeInfo.node, t_supp_read_var_string, SERVICE_NAME_READ_VAR_STRING);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_READ_VAR_STRING, "Failed to init service (%d)", (int)ret);

    ret = rclc_service_init_default(&g_serviceWriteVarByte, &g_microRosNodeInfo.node, t_supp_write_var_byte, SERVICE_NAME_WRITE_VAR_BYTE);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_BYTE, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceWriteVarDouble, &g_microRosNodeInfo.node, t_supp_write_var_double, SERVICE_NAME_WRITE_VAR_DOUBLE);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_DOUBLE, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceWriteVarInteger, &g_microRosNodeInfo.node, t_supp_write_var_integer, SERVICE_NAME_WRITE_VAR_INTEGER);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_INTEGER, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceWriteVarPosition, &g_microRosNodeInfo.node, t_supp_write_var_position, SERVICE_NAME_WRITE_VAR_POSITION);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_POSITION, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceWriteVarReal, &g_microRosNodeInfo.node, t_supp_write_var_real, SERVICE_NAME_WRITE_VAR_REAL);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_REAL, "Failed to init service (%d)", (int)ret);
    ret = rclc_service_init_default(&g_serviceWriteVarString, &g_microRosNodeInfo.node, t_supp_write_var_string, SERVICE_NAME_WRITE_VAR_STRING);
    motoRosAssert_withMsg(ret == RCL_RET_OK, SUBCODE_FAIL_INIT_SERVICE_WRITE_VAR_STRING, "Failed to init service (%d)", (int)ret);


    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_byte_var_read.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_double_var_read.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_int_var_read.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_pos_var_read.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_real_var_read.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_str_var_read.message);

    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_byte_var_write.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_double_var_write.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_int_var_write.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_pos_var_write.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_real_var_write.message);
    rosidl_runtime_c__String__init(&g_messages_ReadWriteVariable.resp_str_var_write.message);

    MOTOROS2_MEM_TRACE_REPORT(svc_rw_var_init);
}

void Ros_ServiceReadWriteVariable_Cleanup()
{
    MOTOROS2_MEM_TRACE_START(svc_rw_var_fini);

    rcl_ret_t ret;

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_BYTE "");
    ret = rcl_service_fini(&g_serviceReadVarByte, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_BYTE " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_DOUBLE "");
    ret = rcl_service_fini(&g_serviceReadVarDouble, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_DOUBLE " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_INTEGER "");
    ret = rcl_service_fini(&g_serviceReadVarInteger, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_INTEGER " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_POSITION "");
    ret = rcl_service_fini(&g_serviceReadVarPosition, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_POSITION " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_REAL "");
    ret = rcl_service_fini(&g_serviceReadVarReal, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_REAL " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_READ_VAR_STRING "");
    ret = rcl_service_fini(&g_serviceReadVarString, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_READ_VAR_STRING " service: %d", ret);


    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_BYTE "");
    ret = rcl_service_fini(&g_serviceWriteVarByte, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_BYTE " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_DOUBLE "");
    ret = rcl_service_fini(&g_serviceWriteVarDouble, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_DOUBLE " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_INTEGER "");
    ret = rcl_service_fini(&g_serviceWriteVarInteger, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_INTEGER " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_POSITION "");
    ret = rcl_service_fini(&g_serviceWriteVarPosition, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_POSITION " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_REAL "");
    ret = rcl_service_fini(&g_serviceWriteVarReal, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_REAL " service: %d", ret);

    Ros_Debug_BroadcastMsg("Cleanup service " SERVICE_NAME_WRITE_VAR_STRING "");
    ret = rcl_service_fini(&g_serviceWriteVarString, &g_microRosNodeInfo.node);
    if (ret != RCL_RET_OK)
        Ros_Debug_BroadcastMsg("Failed cleaning up " SERVICE_NAME_WRITE_VAR_STRING " service: %d", ret);


    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_byte_var_read.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_double_var_read.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_int_var_read.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_pos_var_read.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_real_var_read.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_str_var_read.message);

    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_byte_var_write.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_double_var_write.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_int_var_write.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_pos_var_write.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_real_var_write.message);
    rosidl_runtime_c__String__fini(&g_messages_ReadWriteVariable.resp_str_var_write.message);

    MOTOROS2_MEM_TRACE_REPORT(svc_rw_var_fini);
}


void Ros_ServiceReadVarByte_Trigger(const void* request_msg, void* response_msg)
{
    motoros2_interfaces__srv__ReadVarByte_Request* request =
        (motoros2_interfaces__srv__ReadVarByte_Request*) request_msg;

    motoros2_interfaces__srv__ReadVarByte_Response* response =
        (motoros2_interfaces__srv__ReadVarByte_Response*) response_msg;


    if (Ros_VarServer_IsValidVarNumber(request->var_number))
    {
        STATUS status = ERROR;

        MP_USR_VAR_INFO readVarInfo;
        memset(&readVarInfo, 0, sizeof(readVarInfo));

        readVarInfo.var_type = MK_VAR_TYPE(MP_VAR_B);
        readVarInfo.var_no = request->var_number;
        status = mpGetUserVars(&readVarInfo);

        response->value = readVarInfo.val.b;
        response->result_code = (status == 0) ? VAR_RESULT_OK : VAR_RESULT_READ_API_ERROR;
        response->success = (status == 0);
        rosidl_runtime_c__String__assign(&response->message,
            Ros_VarServer_ResultCodeToStr(response->result_code));
    }
    else
    {
        response->value = 0;
        response->result_code = VAR_RESULT_READ_VAR_NUMBER_INVALID;
        response->success = FALSE;
        rosidl_runtime_c__String__assign(&response->message,
            Ros_VarServer_ResultCodeToStr(response->result_code));
    }
}

void Ros_ServiceWriteVarByte_Trigger(const void* request_msg, void* response_msg)
{
    motoros2_interfaces__srv__WriteVarByte_Request* request =
        (motoros2_interfaces__srv__WriteVarByte_Request*) request_msg;

    motoros2_interfaces__srv__WriteVarByte_Response* response =
        (motoros2_interfaces__srv__WriteVarByte_Response*) response_msg;


    if (Ros_VarServer_IsValidVarNumber(request->var_number))
    {
        STATUS status = ERROR;

        MP_USR_VAR_INFO writeVarInfo;
        memset(&writeVarInfo, 0, sizeof(writeVarInfo));

        writeVarInfo.var_type = MK_VAR_TYPE(MP_VAR_B);
        writeVarInfo.var_no = request->var_number;
        writeVarInfo.val.b = request->value;
        status = mpPutUserVars(&writeVarInfo);

        response->result_code = (status == 0) ? VAR_RESULT_OK : VAR_RESULT_WRITE_API_ERROR;
        response->success = (status == 0);
        rosidl_runtime_c__String__assign(&response->message,
            Ros_VarServer_ResultCodeToStr(response->result_code));
    }
    else
    {
        response->result_code = VAR_RESULT_WRITE_VAR_NUMBER_INVALID;

        response->success = FALSE;
        rosidl_runtime_c__String__assign(&response->message,
            Ros_VarServer_ResultCodeToStr(response->result_code));
    }
}


// empty functions
void Ros_ServiceReadVarDouble_Trigger(const void* request_msg, void* response_msg) {}
void Ros_ServiceWriteVarDouble_Trigger(const void* request_msg, void* response_msg) {}

void Ros_ServiceReadVarInteger_Trigger(const void* request_msg, void* response_msg) {}
void Ros_ServiceWriteVarInteger_Trigger(const void* request_msg, void* response_msg) {}

void Ros_ServiceReadVarPosition_Trigger(const void* request_msg, void* response_msg) {}
void Ros_ServiceWriteVarPosition_Trigger(const void* request_msg, void* response_msg) {}

void Ros_ServiceReadVarReal_Trigger(const void* request_msg, void* response_msg) {}
void Ros_ServiceWriteVarReal_Trigger(const void* request_msg, void* response_msg) {}

void Ros_ServiceReadVarString_Trigger(const void* request_msg, void* response_msg) {}
void Ros_ServiceWriteVarString_Trigger(const void* request_msg, void* response_msg) {}


BOOL Ros_VarServer_IsValidVarNumber(UINT32 var_number)
{
    return TRUE;
}

const char* const Ros_VarServer_ResultCodeToStr(UINT32 resultCode)
{
    //message strings defined in motoros2_interfaces/msg/VarResultCodes.msg
    switch(resultCode)
    {
        case VAR_RESULT_OK:
            return motoros2_interfaces__msg__VarResultCodes__OK_STR;
        case VAR_RESULT_READ_VAR_NUMBER_INVALID:
            return motoros2_interfaces__msg__VarResultCodes__READ_VAR_NUMBER_INVALID_STR;
        case VAR_RESULT_WRITE_VAR_NUMBER_INVALID:
            return motoros2_interfaces__msg__VarResultCodes__WRITE_VAR_NUMBER_INVALID_STR;
        case VAR_RESULT_READ_API_ERROR:
            return motoros2_interfaces__msg__VarResultCodes__READ_API_ERROR;
        case VAR_RESULT_WRITE_API_ERROR:
            return motoros2_interfaces__msg__VarResultCodes__WRITE_API_ERROR;
        default:
            return motoros2_interfaces__msg__VarResultCodes__UNKNOWN_API_ERROR_STR;
    }
}
