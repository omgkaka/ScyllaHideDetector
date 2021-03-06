#include <Windows.h>
#include <winternl.h>
#include <iostream>
//#define AHOOK_LOG
#include "utils/Native.h"
#include "utils/Hash.h"
#include "utils/Helpers.h"
#include "crc32c/crc32c.h"
#include "utils/LengthDisasm.h"
#include <vector>

void ntdll_restore(const char *func_name)
{
  const auto ntdll = GET_MODULE_BASE_ADDRESS(L"ntdll.dll");
  PVOID ntdll_mapped = nullptr;
  MAP_NATIVE_MODULE("ntdll.dll", &ntdll_mapped);
  const auto hooked_func_adress = (uint8_t *)resolve_jmp(get_proc_address(ntdll, HASHSTR(func_name)), 1);
  const auto hooked_func_size = static_cast<size_t>(GetSizeOfProc(hooked_func_adress, 1));
  const auto crc_hooked = crc32c::Crc32c(hooked_func_adress, hooked_func_size);
  const auto original_func_adress = (uint8_t *)resolve_jmp(get_proc_address(ntdll_mapped, HASHSTR(func_name)), 1);
  const auto original_func_size = static_cast<size_t>(GetSizeOfProc(original_func_adress, 1));
  const auto crc_original = crc32c::Crc32c(original_func_adress, original_func_size);
  // detect hook and restore bytes
  if (crc_original != crc_hooked)
  {
#ifndef AHOOK_LOG
    log("[Detect] " + static_cast<std::string>(func_name) + "\r\n");
#endif
    DWORD oldprotect = 0;
    VirtualProtect(hooked_func_adress, hooked_func_size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy(hooked_func_adress, original_func_adress, hooked_func_size);
    VirtualProtect(hooked_func_adress, hooked_func_size, oldprotect, &oldprotect);
  }
  else
  {
#ifndef AHOOK_LOG
    log("[Ok] " + static_cast<std::string>(func_name) + "\r\n");
#endif
  }
}

void kernelbase_restore(const char *func_name)
{
  const auto kernelbase = GET_MODULE_BASE_ADDRESS("kernelbase.dll");
  PVOID kernelbase_mapped = nullptr;
  MAP_NATIVE_MODULE("kernelbase.dll", &kernelbase_mapped);
  const auto hooked_func_adress = (uint8_t *)resolve_jmp(get_proc_address(kernelbase, HASHSTR(func_name)), 1);
  const auto hooked_func_size = static_cast<size_t>(GetSizeOfProc(hooked_func_adress, 1));
  const auto crc_hooked = crc32c::Crc32c(hooked_func_adress, hooked_func_size);
  const auto original_func_adress = (uint8_t *)resolve_jmp(get_proc_address(kernelbase_mapped, HASHSTR(func_name)), 1);
  const auto original_func_size = static_cast<size_t>(GetSizeOfProc(original_func_adress, 1));
  const auto crc_original = crc32c::Crc32c(original_func_adress, original_func_size);
  // detect hook and restore bytes
  if (crc_original != crc_hooked)
  {
#ifndef AHOOK_LOG
    log("[Detect] " + static_cast<std::string>(func_name) + "\r\n");
#endif
    DWORD oldprotect = 0;
    VirtualProtect(hooked_func_adress, hooked_func_size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy(hooked_func_adress, original_func_adress, hooked_func_size);
    VirtualProtect(hooked_func_adress, hooked_func_size, oldprotect, &oldprotect);
  }
  else
  {
#ifndef AHOOK_LOG
    log("[Ok] " + static_cast<std::string>(func_name) + "\r\n");
#endif
  }
}

void user32_restore(const char *func_name)
{
  // TODO: Test on Win7,8
  if (getSysOpType() == 10)
  {
    const auto h_module = LoadLibraryW(L"user32.dll");
    const auto win32_u = GET_MODULE_BASE_ADDRESS("win32u.dll");
    PVOID win32_u_mapped = nullptr;
    MAP_NATIVE_MODULE("win32u.dll", &win32_u_mapped);
    const auto hooked_func_adress = (uint8_t *)resolve_jmp(get_proc_address(win32_u, HASHSTR(func_name)), 1);
    const auto hooked_func_size = static_cast<size_t>(GetSizeOfProc(hooked_func_adress, 1));
    const auto crc_hooked = crc32c::Crc32c(hooked_func_adress, hooked_func_size);
    const auto original_func_adress = (uint8_t *)resolve_jmp(get_proc_address(win32_u_mapped, HASHSTR(func_name)), 1);
    const auto original_func_size = static_cast<size_t>(GetSizeOfProc(original_func_adress, 1));
    const auto crc_original = crc32c::Crc32c(original_func_adress, original_func_size);
    // detect hook and restore bytes
    if (crc_original != crc_hooked)
    {
#ifndef AHOOK_LOG
      log("[Detect] " + static_cast<std::string>(func_name) + "\r\n");
#endif
      DWORD oldprotect = 0;
      VirtualProtect(hooked_func_adress, hooked_func_size, PAGE_EXECUTE_READWRITE, &oldprotect);
      memcpy(hooked_func_adress, original_func_adress, hooked_func_size);
      VirtualProtect(hooked_func_adress, hooked_func_size, oldprotect, &oldprotect);
    }
    else
    {
#ifndef AHOOK_LOG
      log("[Ok] " + static_cast<std::string>(func_name) + "\r\n");
#endif
    }
    FreeLibrary(h_module);
  }
  else
  {
    const auto h_module = LoadLibraryW(L"user32.dll");
    const auto user_32 = GET_MODULE_BASE_ADDRESS(L"user32.dll");
    PVOID user32_mapped = nullptr;
    MAP_NATIVE_MODULE("user32.dll", &user32_mapped);
    const auto hooked_func_adress = (uint8_t *)resolve_jmp(get_proc_address(user_32, HASHSTR(func_name)), 1);
    const auto hooked_func_size = static_cast<size_t>(GetSizeOfProc(hooked_func_adress, 1));
    const auto crc_hooked = crc32c::Crc32c(hooked_func_adress, hooked_func_size);
    const auto original_func_adress = (uint8_t *)resolve_jmp(get_proc_address(user32_mapped, HASHSTR(func_name)), 1);
    const auto original_func_size = static_cast<size_t>(GetSizeOfProc(original_func_adress, 1));
    const auto crc_original = crc32c::Crc32c(original_func_adress, original_func_size);
    // detect hook and restore bytes
    if (crc_original != crc_hooked)
    {
#ifndef AHOOK_LOG
      log("[Detect] " + static_cast<std::string>(func_name) + "\r\n");
#endif
      DWORD oldprotect = 0;
      VirtualProtect(hooked_func_adress, hooked_func_size, PAGE_EXECUTE_READWRITE, &oldprotect);
      memcpy(hooked_func_adress, original_func_adress, hooked_func_size);
      VirtualProtect(hooked_func_adress, hooked_func_size, oldprotect, &oldprotect);
    }
    else
    {
#ifndef AHOOK_LOG
      log("[Ok] " + static_cast<std::string>(func_name) + "\r\n");
#endif
    }
    FreeLibrary(h_module);
  }
}

int main()
{
  ntdll_restore("NtYieldExecution");
  ntdll_restore("NtSetInformationThread");
  ntdll_restore("NtSetInformationProcess");
  ntdll_restore("NtQuerySystemInformation");
  ntdll_restore("NtQueryInformationProcess");
  ntdll_restore("NtQueryObject");
  ntdll_restore("NtCreateThreadEx");
  ntdll_restore("NtSetDebugFilterState");
  ntdll_restore("NtClose");
  ntdll_restore("NtQueryPerformanceCounter");
  ntdll_restore("NtGetContextThread");
  ntdll_restore("NtSetContextThread");
  //TODO: make this workable
  //ntdll_restore("NtQuerySystemTime");
  kernelbase_restore("GetTickCount");
  kernelbase_restore("GetTickCount64");
  kernelbase_restore("OutputDebugStringA");
  kernelbase_restore("GetLocalTime");
  kernelbase_restore("GetSystemTime");
  user32_restore("NtUserBlockInput");
  user32_restore("NtUserQueryWindow");
  user32_restore("NtUserFindWindowEx");
  user32_restore("NtUserBuildHwndList");
  // additional
  user32_restore("BlockInput");
  user32_restore("FindWindowA");
  kernelbase_restore("CheckRemoteDebuggerPresent");
  system("pause");
  return 0;
}
