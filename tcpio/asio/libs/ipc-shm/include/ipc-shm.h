/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2018-2021 NXP
 */
#ifndef IPC_SHM_H
#define IPC_SHM_H

/*
 * Maximum number of shared memory channels that can be configured
 */
#define IPC_SHM_MAX_CHANNELS 8

/*
 * Maximum number of buffer pools that can be configured for a managed channel
 */
#define IPC_SHM_MAX_POOLS 4

/*
 * Maximum number of buffers per pool
 */
#define IPC_SHM_MAX_BUFS_PER_POOL 4096u

/*
 * Used when polling is desired on either transmit or receive path
 */
#define IPC_IRQ_NONE -1

/**
 * enum ipc_shm_channel_type - channel type
 * @IPC_SHM_MANAGED:	channel with buffer management enabled
 * @IPC_SHM_UNMANAGED:	buf mgmt disabled, app owns entire channel memory
 *
 * For unmanaged channels the application has full control over channel memory
 * and no buffer management is done by ipc-shm device.
 */
enum ipc_shm_channel_type {
	IPC_SHM_MANAGED,
	IPC_SHM_UNMANAGED
};

/**
 * enum ipc_shm_core_type - core type
 * @IPC_CORE_A53:	ARM Cortex-A53 core
 * @IPC_CORE_M7:	ARM Cortex-M7 core
 * @IPC_CORE_M4:	ARM Cortex-M4 core
 * @IPC_CORE_Z7:	PowerPC e200z7 core
 * @IPC_CORE_Z4:	PowerPC e200z4 core
 * @IPC_CORE_Z2:	PowerPC e200z2 core
 * @IPC_CORE_DEFAULT:	used for letting driver auto-select remote core type
 */
enum ipc_shm_core_type {
	IPC_CORE_A53,
	IPC_CORE_M7,
	IPC_CORE_M4,
	IPC_CORE_Z7,
	IPC_CORE_Z4,
	IPC_CORE_Z2,
	IPC_CORE_DEFAULT,
};

/**
 * enum ipc_shm_core_index - core index
 * @IPC_CORE_INDEX_0:	Processor index 0
 * @IPC_CORE_INDEX_1:	Processor index 1
 * @IPC_CORE_INDEX_2:	Processor index 2
 * @IPC_CORE_INDEX_3:	Processor index 3
 */
enum ipc_shm_core_index {
	IPC_CORE_INDEX_0 = 0x01u,
	IPC_CORE_INDEX_1 = 0x02u,
	IPC_CORE_INDEX_2 = 0x04u,
	IPC_CORE_INDEX_3 = 0x08u,
};

/**
 * struct ipc_shm_pool_cfg - memory buffer pool parameters
 * @num_bufs:	number of buffers
 * @buf_size:	buffer size
 */
struct ipc_shm_pool_cfg {
	uint16_t num_bufs;
	uint32_t buf_size;
};

/**
 * struct ipc_shm_managed_cfg - managed channel parameters
 * @num_pools:	number of buffer pools
 * @pools:	memory buffer pools parameters
 * @rx_cb:	receive callback
 * @cb_arg:	optional receive callback argument
 */
struct ipc_shm_managed_cfg {
	int num_pools;
	struct ipc_shm_pool_cfg *pools;
	void (*rx_cb)(void *cb_arg, int chan_id, void *buf, size_t size);
	void *cb_arg;
};

/**
 * struct ipc_shm_unmanaged_cfg - unmanaged channel parameters
 * @size:	unmanaged channel memory size
 * @rx_cb:	receive callback
 * @cb_arg:	optional receive callback argument
 */
struct ipc_shm_unmanaged_cfg {
	uint32_t size;
	void (*rx_cb)(void *cb_arg, int chan_id, void *mem);
	void *cb_arg;
};

/**
 * struct ipc_shm_channel_cfg - channel parameters
 * @type:	channel type from &enum ipc_shm_channel_type
 * @ch.managed:     managed channel parameters
 * @ch.unmanaged:   unmanaged channel parameters
 */
struct ipc_shm_channel_cfg {
	enum ipc_shm_channel_type type;
	union {
		struct ipc_shm_managed_cfg managed;
		struct ipc_shm_unmanaged_cfg unmanaged;
	} ch;
};

/**
 * struct ipc_shm_remote_core - remote core type and index
 * @type:	core type from &enum ipc_shm_core_type
 * @index:	core number
 *
 * Core type can be IPC_CORE_DEFAULT, in which case core index doesn't matter
 * because it's chosen automatically by the driver.
 */
struct ipc_shm_remote_core {
	enum ipc_shm_core_type type;
	enum ipc_shm_core_index index;
};

/**
 * struct ipc_shm_local_core - local core type, index and trusted cores
 * @type:	core type from &enum ipc_shm_core_type
 * @index:	core number targeted by remote core interrupt
 * @trusted:    trusted cores mask
 *
 * Core type can be IPC_CORE_DEFAULT, in which case core index doesn't matter
 * because it's chosen automatically by the driver.
 *
 * Trusted cores mask specifies which cores (of the same core type) have access
 * to the inter-core interrupt status register of the targeted core. The mask
 * can be formed from &enum ipc_shm_core_index.
 */
struct ipc_shm_local_core {
	enum ipc_shm_core_type type;
	enum ipc_shm_core_index index;
	uint32_t trusted;
};

/**
 * struct ipc_shm_cfg - IPC shm parameters
 * @local_shm_addr:	local shared memory physical address
 * @remote_shm_addr:	remote shared memory physical address
 * @shm_size:		local/remote shared memory size
 * @inter_core_tx_irq:	inter-core interrupt reserved for shm driver Tx
 * @inter_core_rx_irq:	inter-core interrupt reserved for shm driver Rx
 * @local_core:		local core targeted by remote core interrupt
 * @remote_core:	remote core to trigger the interrupt on
 * @num_channels:	number of shared memory channels
 * @channels:		IPC channels' parameters array
 *
 * The TX and RX interrupts used must be different. For ARM platforms, a default
 * value can be assigned to the local and remote core using IPC_CORE_DEFAULT.
 * Local core is only used for platforms on which Linux may be running on
 * multiple cores, and is ignored for RTOS and baremetal implementations.
 *
 * Local and remote channel and buffer pool configurations must be symmetric.
 */
struct ipc_shm_cfg {
	uintptr_t local_shm_addr;
	uintptr_t remote_shm_addr;
	uint32_t shm_size;
	int inter_core_tx_irq;
	int inter_core_rx_irq;
	struct ipc_shm_local_core local_core;
	struct ipc_shm_remote_core remote_core;
	int num_channels;
	struct ipc_shm_channel_cfg *channels;
};

/**
 * ipc_shm_init() - initialize shared memory device
 * @cfg:         configuration parameters
 *
 * Function is non-reentrant.
 *
 * Return: 0 on success, error code otherwise
 */
int ipc_shm_init(const struct ipc_shm_cfg *cfg);

/**
 * ipc_shm_free() - release shared memory device
 *
 * Function is non-reentrant.
 */
void ipc_shm_free(void);

/**
 * ipc_shm_acquire_buf() - request a buffer for the given channel
 * @chan_id:        channel index
 * @size:           required size
 *
 * Function used only for managed channels where buffer management is enabled.
 * Function is thread-safe for different channels but not for the same channel.
 *
 * Return: pointer to the buffer base address or NULL if buffer not found
 */
void *ipc_shm_acquire_buf(int chan_id, size_t size);

/**
 * ipc_shm_release_buf() - release a buffer for the given channel
 * @chan_id:        channel index
 * @buf:            buffer pointer
 *
 * Function used only for managed channels where buffer management is enabled.
 * Function is thread-safe for different channels but not for the same channel.
 *
 * Return: 0 on success, error code otherwise
 */
int ipc_shm_release_buf(int chan_id, const void *buf);

/**
 * ipc_shm_tx() - send data on given channel and notify remote
 * @chan_id:        channel index
 * @buf:            buffer pointer
 * @size:           size of data written in buffer
 *
 * Function used only for managed channels where buffer management is enabled.
 * Function is thread-safe for different channels but not for the same channel.
 *
 * Return: 0 on success, error code otherwise
 */
int ipc_shm_tx(int chan_id, void *buf, size_t size);

/**
 * ipc_shm_unmanaged_acquire() - acquire the unmanaged channel local memory
 * @chan_id:        channel index
 *
 * Function used only for unmanaged channels. The memory must be acquired only
 * once after the channel is initialized. There is no release function needed.
 * Function is thread-safe for different channels but not for the same channel.
 *
 * Return: pointer to the channel memory or NULL if invalid channel
 */
void *ipc_shm_unmanaged_acquire(int chan_id);

/**
 * ipc_shm_unmanaged_tx() - notify remote that data has been written in channel
 * @chan_id:        channel index
 *
 * Function used only for unmanaged channels. It can be used after the channel
 * memory has been acquired whenever is needed to signal remote that new data
 * is available in channel memory.
 * Function is thread-safe for different channels but not for the same channel.
 *
 * Return: 0 on success, error code otherwise
 */
int ipc_shm_unmanaged_tx(int chan_id);

/**
 * ipc_shm_is_remote_ready() - check whether remote is initialized
 *
 * Function used to check if the remote is initialized and ready to receive
 * messages. It should be invoked at least before the first transmit operation.
 *
 * Return: 0 if remote is initialized, error code otherwise
 */
int ipc_shm_is_remote_ready(void);

/**
 * ipc_shm_poll_channels() - poll the channels for available messages to process
 *
 * This function handles all channels using a fair handling algorithm: all
 * channels are treated equally and no channel is starving.
 *
 * Return: number of messages processed, error code otherwise
 */
int ipc_shm_poll_channels(void);

#endif /* IPC_SHM_H */
