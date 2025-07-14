# The list of sysmond process attributes

| ID | Call | Type | Structure | Field |
|----|------|------|-----------|-------|
| 1 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_flags |
| 2 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_status |
| 3 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_xstatus |
| 4 | proc_listpids() | uint64 | | PID |
| 5 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_ppid |
| 6 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_uid |
| 7 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_gid |
| 8 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_ruid |
| 9 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_rgid |
| 0xA | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_svuid |
| 0xB | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_svgid |
| 0xC | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | string | proc_bsdinfowithuniqid | pbsd.pbi_comm |
| 0xD | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | string | proc_bsdinfowithuniqid | pbsd.pbi_name |
| 0xE | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_nfiles |
| 0xF | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_pgid |
| 0x10 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.pbi_pjobc |
| 0x11 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint32 | proc_bsdinfowithuniqid | pbsd.e_tdev |
| 0x12 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbsd.e_tpgid |
| 0x13 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint32 | proc_bsdinfowithuniqid | pbsd.pbi_nice |
| 0x14 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | date | proc_bsdinfowithuniqid | pbsd.pbi_start_tvusec * 1000 + pbsd.pbi_start_tvsec * 1000000000 |
| 0x15 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | info.pti_virtual_size |
| 0x16 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | info.pti_resident_size |
| 0x17 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | ticks_to_ns(info.pti_total_user) |
| 0x18 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | ticks_to_ns(info.pti_total_system) |
| 0x19 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | ticks_to_ns(info.pti_threads_user) |
| 0x1A | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | ticks_to_ns(info.pti_threads_system) |
| 0x1B | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_policy |
| 0x1C | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_faults |
| 0x1D | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint64 | proc_taskinfo | info.pti_pageins |
| 0x1E | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_cow_faults |
| 0x1F | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_messages_sent |
| 0x20 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_messages_received |
| 0x21 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_syscalls_mach |
| 0x22 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_syscalls_unix |
| 0x23 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_csw |
| 0x24 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_threadnum |
| 0x25 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_numrunning |
| 0x26 | proc_pidinfo() flavor=PROC_PIDTASKINFO | uint32 | proc_taskinfo | info.pti_priority |
| 0x27 | proc_pidonfo() flavor=PROC_PIDWORKQUEUEINFO | uint64 | proc_workqueueinfo | info.pwq_nthreads |
| 0x28 | proc_pidonfo() flavor=PROC_PIDWORKQUEUEINFO | uint64 | proc_workqueueinfo | info.pwq_runthreads |
| 0x29 | proc_pidonfo() flavor=PROC_PIDWORKQUEUEINFO | uint64 | proc_workqueueinfo | info.pwq_blockedthreads |
| 0x2A | proc_pidonfo() flavor=PROC_PIDWORKQUEUEINFO | uint64 | proc_workqueueinfo | info.pwq_state |
| 0x2B | sysctl() MIB=int {0x1, 0x31, PID} | string | | command with arguments |
| 0x2C | sysctl() MIB=int {0x1, 0x31, PID} | string | | processname |
| 0x2E | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_interrupt_wkups |
| 0x2F | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_pkg_idle_wkups |
| 0x30 | task_info() flavor=TASK_POWER_INFO | uint64 | task_power_info | task_info_out.task_timer_wakeups_bin_1 |
| 0x31 | task_info() flavor=TASK_POWER_INFO | uint64 | task_power_info | task_info_out.task_timer_wakeups_bin_2 |
| 0x32 | mach_port_space_basic_info() | uint64 | ipc_info_space_basic_t | basic_info.iisb_table_inuse |
| 0x37 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | pbi_ppid |
| 0x38 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | boolean | proc_bsdinfowithuniqid | (pbsd.pbi_flags >> 0x17) & 1 |
| 0x39 | task_policy_get() flavor=TASK_BASE_QOS_POLICY | uint32 | task_qos_policy | policy_info.task_latency_qos_tier |
| 0x3A | task_policy_get() flavor=TASK_BASE_QOS_POLICY | uint32 | task_qos_policy | policy_info.task_throughput_qos_tier |
| 0x3B | _task_info() flavor=TASK_VM_INFO_PURGEABLE | uint64 | task_vm_info | task_info.purgeable_volatile_resident |
| 0x3C | pm_energy_impact() | uint64 | pm_task_energy_data_t | ??? |
| 0x3D | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | cpu_ticks_to_ns(mach_absolute_time() - rusage.ri_proc_start_abstime) |
| 0x3E | pm_energy_impact() | double | pm_task_energy_data_t | ??? |
| 0x40 | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uuid | rusage_info_v2 | rusage.ri_uuid |
| 0x41 | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_wired_size |
| 0x42 | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_phys_footprint |
| 0x43 | _task_info() flavor=TASK_VM_INFO_PURGEABLE | uint64 | task_vm_info | task_info.compressed + task_info.internal - task_info.purgeable_volatile_pmap |
| 0x44 | _task_info() flavor=TASK_VM_INFO_PURGEABLE | uint64 | task_vm_info | task_info.internal_peak |
| 0x45 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | p_uniqidentifier.p_uniqueid |
| 0x46 | proc_pidinfo() flavor=PROC_PIDT_BSDINFOWITHUNIQID | uint64 | proc_bsdinfowithuniqid | p_uniqidentifier.p_puniqueid |
| 0x47 | responsibility_get_uniqueid_responsible_for_pid() | uint64 | | |
| 0x48 | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_diskio_bytesread |
| 0x49 | proc_pid_rusage() flavor=RUSAGE_INFO_V2 | uint64 | rusage_info_v2 | rusage.ri_diskio_byteswritten |
| 0x4A | _task_info() flavor=TASK_VM_INFO_PURGEABLE | uint64 | task_vm_info | task_info.compressed |
| 0x4B | _proc_pidinfo() flavor=PROC_PIDCOALITIONINFO | uint64 | proc_pidcoalitioninfo | coalition.coalition_id[0] |
