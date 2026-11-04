#ifndef PAPACC_PERSONAL_SCQ_INPUTS_WIN32_H
#define PAPACC_PERSONAL_SCQ_INPUTS_WIN32_H
#include <windows.h>
#include "server_store_hash_win32.h"
#include "personal_scq_profile_win32.h"
#include "personal_runtime_identity_win32.h"
#include "personal_install_paths_win32.h"
#include <winternl.h>
#include "personal_scq_native_sd_win32.h"
/* Bounded reference custody in this existing input packet. Mirrors the
 * existing eight-slot directory-chain representation; not a quota/grant. */
#define PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS 8U
typedef struct PAPACC_SCQ_IMAGE_CHAIN {
 struct PAPACC_SCQ_IMAGE_CHAIN *self;const void *owner;volatile LONG *stop;
 const PAPACC_PERSONAL_INSTALL_PATHS *paths;
 HANDLE directory[PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS],image;
 BY_HANDLE_FILE_INFORMATION identity[PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS+1U];
 PSECURITY_DESCRIPTOR sd[PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS+1U],scratch;PSID installer;
 DWORD length[PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS+1U],thread,count,phase,first,close_attempted;
 PAPACC_SCQ_PLAN_DATA plan; /* frozen typed discovery scope, NOT authority */
 IO_STATUS_BLOCK io;OBJECT_ATTRIBUTES attributes;UNICODE_STRING unicode;NTSTATUS open_status;
 WCHAR binary[MAX_PATH]; /* frozen known-folder target, not a reopen source */
 WCHAR name[MAX_PATH+4]; /* durable backing on unexpected pending open */
 PAPACC_STORE_HASH target_hash;BYTE target_bytes[572]; /* P3 canonical private preimage */
 PAPACC_SCQ_NATIVE_SD_STATUS native_sd;
} PAPACC_SCQ_IMAGE_CHAIN;
/* Custody of ORIGINAL REFERENCES only. Never an authorization/grant and never
 * a source reconstructed from a receipt. No path, secret or content output. */
typedef struct PAPACC_PERSONAL_SCQ_INPUTS {
 PAPACC_SCQ_IMAGE_CHAIN image_chain;
 HANDLE reader[3];PAPACC_STORE_HASH content_hash;BYTE digest[3][32],descriptor_digest[3][32];DWORD content_verified;
 HANDLE file[3];BY_HANDLE_FILE_INFORMATION identity[3];
 PSECURITY_DESCRIPTOR descriptor[3],checking;
 DWORD descriptor_bytes[3],thread,phase,first,error,close_attempts;
 PAPACC_SCQ_NATIVE_SD_STATUS native_sd;
} PAPACC_PERSONAL_SCQ_INPUTS;
#endif
