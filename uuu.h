//
// Created by djkri on 8/6/2024.
//

#ifndef UUU_H
#define UUU_H

#endif //UUU_H

// define varibles and structs we're using

typedef struct _PS_ATTRIBUTE
{
    ULONG_PTR Attribute;
    SIZE_T Size;
    union
    {
        ULONG_PTR Value;
        PVOID ValuePtr;
    };
    PSIZE_T ReturnLength;
} PS_ATTRIBUTE, *PPS_ATTRIBUTE;

typedef struct _PS_ATTRIBUTE_LIST
{
    SIZE_T TotalLength;
    PS_ATTRIBUTE Attributes[1];
} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;

typedef NTSTATUS (NTAPI *PUSER_THREAD_START_ROUTINE)(
    _In_ PVOID ThreadParameter
    );

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    _Field_size_bytes_part_opt_(MaximumLength, Length) PWCH Buffer;
} UNICODE_STRING, *PUNICODE_STRING;


typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor; // PSECURITY_DESCRIPTOR;
    PVOID SecurityQualityOfService; // PSECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

//define the different functions

typedef NTSTATUS(NTAPI *NtAllocateVirtualMemory)(
_In_ HANDLE ProcessHandle,
_Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID *BaseAddress,
_In_ ULONG_PTR ZeroBits,
_Inout_ PSIZE_T RegionSize,
_In_ ULONG AllocationType,
_In_ ULONG Protect
);



typedef NTSTATUS(NTAPI *NtClose)(
_In_ HANDLE ProcessHandle
);

typedef NTSTATUS(NTAPI *NtOpenProcess)(
_Out_ PHANDLE ProcessHandle,
_In_ ACCESS_MASK DesiredAccess,
_In_ POBJECT_ATTRIBUTES ObjectAttributes,
_In_opt_ PCLIENT_ID ClientId
);

typedef NTSTATUS(NTAPI *NtCreateThreadEx)(
_Out_ PHANDLE ThreadHandle,
_In_ ACCESS_MASK DesiredAccess,
_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
_In_ HANDLE ProcessHandle,
_In_ PUSER_THREAD_START_ROUTINE StartRoutine,
_In_opt_ PVOID Argument,
_In_ ULONG CreateFlags, // THREAD_CREATE_FLAGS_*
_In_ SIZE_T ZeroBits,
_In_ SIZE_T StackSize,
_In_ SIZE_T MaximumStackSize,
_In_opt_ PPS_ATTRIBUTE_LIST AttributeList);

typedef NTSTATUS(NTAPI *NtWriteVirtualMemory)(
_In_ HANDLE ProcessHandle,
_In_opt_ PVOID BaseAddress,
_In_reads_bytes_(BufferSize) PVOID Buffer,
_In_ SIZE_T BufferSize,
_Out_opt_ PSIZE_T NumberOfBytesWritten
    );
