/*
 * Copyright (c) 2000-2016 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	From: @(#)if.h	8.1 (Berkeley) 6/10/93
 * $FreeBSD: src/sys/net/if_var.h,v 1.18.2.7 2001/07/24 19:10:18 brooks Exp $
 */

#ifndef	_NET_IF_VAR_H_
#define	_NET_IF_VAR_H_

#include <sys/appleapiopts.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>		/* get TAILQ macros */
#include <kern/locks.h>
#include <net/route.h>

#include <net/kpi_interface.h>

#ifdef __APPLE__
#define APPLE_IF_FAM_LOOPBACK  1
#define APPLE_IF_FAM_ETHERNET  2
#define APPLE_IF_FAM_SLIP      3
#define APPLE_IF_FAM_TUN       4
#define APPLE_IF_FAM_VLAN      5
#define APPLE_IF_FAM_PPP       6
#define APPLE_IF_FAM_PVC       7
#define APPLE_IF_FAM_DISC      8
#define APPLE_IF_FAM_MDECAP    9
#define APPLE_IF_FAM_GIF       10
#define APPLE_IF_FAM_FAITH     11	/* deprecated */
#define APPLE_IF_FAM_STF       12
#define APPLE_IF_FAM_FIREWIRE  13
#define APPLE_IF_FAM_BOND      14
#endif /* __APPLE__ */

/*
 * 72 was chosen below because it is the size of a TCP/IP
 * header (40) + the minimum mss (32).
 */
#define	IF_MINMTU	72
#define	IF_MAXMTU	65535

/*
 * Structures defining a network interface, providing a packet
 * transport mechanism (ala level 0 of the PUP protocols).
 *
 * Each interface accepts output datagrams of a specified maximum
 * length, and provides higher level routines with input datagrams
 * received from its medium.
 *
 * Output occurs when the routine if_output is called, with three parameters:
 *	(*ifp->if_output)(ifp, m, dst, rt)
 * Here m is the mbuf chain to be sent and dst is the destination address.
 * The output routine encapsulates the supplied datagram if necessary,
 * and then transmits it on its medium.
 *
 * On input, each interface unwraps the data received by it, and either
 * places it on the input queue of a internetwork datagram routine
 * and posts the associated software interrupt, or passes the datagram to a raw
 * packet input routine.
 *
 * Routines exist for locating interfaces by their addresses
 * or for locating a interface on a certain network, as well as more general
 * routing and gateway routines maintaining information used to locate
 * interfaces.  These routines live in the files if.c and route.c
 */

#define	IFNAMSIZ	16

/* This belongs up in socket.h or socketvar.h, depending on how far the
 *   event bubbles up.
 */

struct net_event_data {
	u_int32_t	if_family;
	u_int32_t	if_unit;
	char		if_name[IFNAMSIZ];
};

#if defined(__LP64__)
#include <sys/_types/_timeval32.h>
#define IF_DATA_TIMEVAL timeval32
#else
#define IF_DATA_TIMEVAL timeval
#endif

#pragma pack(4)

/*
 * Structure describing information about an interface
 * which may be of interest to management entities.
 */
struct if_data {
	/* generic interface information */
	u_char		ifi_type;	/* ethernet, tokenring, etc */
	u_char		ifi_typelen;	/* Length of frame type id */
	u_char		ifi_physical;	/* e.g., AUI, Thinnet, 10base-T, etc */
	u_char		ifi_addrlen;	/* media address length */
	u_char		ifi_hdrlen;	/* media header length */
	u_char		ifi_recvquota;	/* polling quota for receive intrs */
	u_char		ifi_xmitquota;	/* polling quota for xmit intrs */
	u_char		ifi_unused1;	/* for future use */
	u_int32_t	ifi_mtu;	/* maximum transmission unit */
	u_int32_t	ifi_metric;	/* routing metric (external only) */
	u_int32_t	ifi_baudrate;	/* linespeed */
	/* volatile statistics */
	u_int32_t	ifi_ipackets;	/* packets received on interface */
	u_int32_t	ifi_ierrors;	/* input errors on interface */
	u_int32_t	ifi_opackets;	/* packets sent on interface */
	u_int32_t	ifi_oerrors;	/* output errors on interface */
	u_int32_t	ifi_collisions;	/* collisions on csma interfaces */
	u_int32_t	ifi_ibytes;	/* total number of octets received */
	u_int32_t	ifi_obytes;	/* total number of octets sent */
	u_int32_t	ifi_imcasts;	/* packets received via multicast */
	u_int32_t	ifi_omcasts;	/* packets sent via multicast */
	u_int32_t	ifi_iqdrops;	/* dropped on input, this interface */
	u_int32_t	ifi_noproto;	/* destined for unsupported protocol */
	u_int32_t	ifi_recvtiming;	/* usec spent receiving when timing */
	u_int32_t	ifi_xmittiming;	/* usec spent xmitting when timing */
	struct IF_DATA_TIMEVAL ifi_lastchange;	/* time of last administrative change */
	u_int32_t	ifi_unused2;	/* used to be the default_proto */
	u_int32_t	ifi_hwassist;	/* HW offload capabilities */
	u_int32_t	ifi_reserved1;	/* for future use */
	u_int32_t	ifi_reserved2;	/* for future use */
};

/*
 * Structure describing information about an interface
 * which may be of interest to management entities.
 */
struct if_data64 {
	/* generic interface information */
	u_char		ifi_type;		/* ethernet, tokenring, etc */
	u_char		ifi_typelen;		/* Length of frame type id */
	u_char		ifi_physical;		/* e.g., AUI, Thinnet, 10base-T, etc */
	u_char		ifi_addrlen;		/* media address length */
	u_char		ifi_hdrlen;		/* media header length */
	u_char		ifi_recvquota;		/* polling quota for receive intrs */
	u_char		ifi_xmitquota;		/* polling quota for xmit intrs */
	u_char		ifi_unused1;		/* for future use */
	u_int32_t	ifi_mtu;		/* maximum transmission unit */
	u_int32_t	ifi_metric;		/* routing metric (external only) */
	u_int64_t	ifi_baudrate;		/* linespeed */
	/* volatile statistics */
	u_int64_t	ifi_ipackets;		/* packets received on interface */
	u_int64_t	ifi_ierrors;		/* input errors on interface */
	u_int64_t	ifi_opackets;		/* packets sent on interface */
	u_int64_t	ifi_oerrors;		/* output errors on interface */
	u_int64_t	ifi_collisions;		/* collisions on csma interfaces */
	u_int64_t	ifi_ibytes;		/* total number of octets received */
	u_int64_t	ifi_obytes;		/* total number of octets sent */
	u_int64_t	ifi_imcasts;		/* packets received via multicast */
	u_int64_t	ifi_omcasts;		/* packets sent via multicast */
	u_int64_t	ifi_iqdrops;		/* dropped on input, this interface */
	u_int64_t	ifi_noproto;		/* destined for unsupported protocol */
	u_int32_t	ifi_recvtiming;		/* usec spent receiving when timing */
	u_int32_t	ifi_xmittiming;		/* usec spent xmitting when timing */
	struct IF_DATA_TIMEVAL ifi_lastchange;	/* time of last administrative change */
};

struct if_traffic_class {
	u_int64_t		ifi_ibepackets;	/* TC_BE packets received on interface */
	u_int64_t		ifi_ibebytes;	/* TC_BE bytes received on interface */
	u_int64_t		ifi_obepackets;	/* TC_BE packet sent on interface */
	u_int64_t		ifi_obebytes;	/* TC_BE bytes sent on interface */
	u_int64_t		ifi_ibkpackets;	/* TC_BK packets received on interface */
	u_int64_t		ifi_ibkbytes;	/* TC_BK bytes received on interface */
	u_int64_t		ifi_obkpackets;	/* TC_BK packet sent on interface */
	u_int64_t		ifi_obkbytes;	/* TC_BK bytes sent on interface */
	u_int64_t		ifi_ivipackets;	/* TC_VI packets received on interface */
	u_int64_t		ifi_ivibytes;	/* TC_VI bytes received on interface */
	u_int64_t		ifi_ovipackets;	/* TC_VI packets sent on interface */
	u_int64_t		ifi_ovibytes;	/* TC_VI bytes sent on interface */
	u_int64_t		ifi_ivopackets;	/* TC_VO packets received on interface */
	u_int64_t		ifi_ivobytes;	/* TC_VO bytes received on interface */
	u_int64_t		ifi_ovopackets;	/* TC_VO packets sent on interface */
	u_int64_t		ifi_ovobytes;	/* TC_VO bytes sent on interface */
	u_int64_t		ifi_ipvpackets;	/* TC priv packets received on interface */
	u_int64_t		ifi_ipvbytes;	/* TC priv bytes received on interface */
	u_int64_t		ifi_opvpackets;	/* TC priv packets sent on interface */
	u_int64_t		ifi_opvbytes;	/* TC priv bytes sent on interface */
};

struct if_data_extended {
	u_int64_t	ifi_alignerrs;	/* unaligned (32-bit) input pkts */
	u_int64_t	ifi_dt_bytes;	/* Data threshold counter */
	u_int64_t	ifi_fpackets;	/* forwarded packets on interface */
	u_int64_t	ifi_fbytes;	/* forwarded bytes on interface */
	u_int64_t	reserved[12];	/* for future */
};

struct if_packet_stats {
	/* TCP */
	u_int64_t		ifi_tcp_badformat;
	u_int64_t		ifi_tcp_unspecv6;
	u_int64_t		ifi_tcp_synfin;
	u_int64_t		ifi_tcp_badformatipsec;
	u_int64_t		ifi_tcp_noconnnolist;
	u_int64_t		ifi_tcp_noconnlist;
	u_int64_t		ifi_tcp_listbadsyn;
	u_int64_t		ifi_tcp_icmp6unreach;
	u_int64_t		ifi_tcp_deprecate6;
	u_int64_t		ifi_tcp_rstinsynrcv;
	u_int64_t		ifi_tcp_ooopacket;
	u_int64_t		ifi_tcp_dospacket;
	u_int64_t		ifi_tcp_cleanup;
	u_int64_t		ifi_tcp_synwindow;
	u_int64_t		reserved[6];
	/* UDP */
	u_int64_t		ifi_udp_port_unreach;
	u_int64_t		ifi_udp_faithprefix;
	u_int64_t		ifi_udp_port0;
	u_int64_t		ifi_udp_badlength;
	u_int64_t		ifi_udp_badchksum;
	u_int64_t		ifi_udp_badmcast;
	u_int64_t		ifi_udp_cleanup;
	u_int64_t		ifi_udp_badipsec;
	u_int64_t		_reserved[4];
};

struct if_description {
	u_int32_t	ifd_maxlen;	/* must be IF_DESCSIZE */
	u_int32_t	ifd_len;	/* actual ifd_desc length */
	u_int8_t	*ifd_desc;	/* ptr to desc buffer */
};

struct if_bandwidths {
	u_int64_t	eff_bw;		/* effective bandwidth */
	u_int64_t	max_bw;		/* maximum theoretical bandwidth */
};

struct if_latencies {
	u_int64_t	eff_lt;		/* effective latency */
	u_int64_t	max_lt;		/* maximum theoretical latency */
};

struct if_rxpoll_stats {
	u_int32_t	ifi_poll_off_req;	/* total # of POLL_OFF reqs */
	u_int32_t	ifi_poll_off_err;	/* total # of POLL_OFF errors */
	u_int32_t	ifi_poll_on_req;	/* total # of POLL_ON reqs */
	u_int32_t	ifi_poll_on_err;	/* total # of POLL_ON errors */

	u_int32_t	ifi_poll_wakeups_avg;	/* avg # of wakeup reqs */
	u_int32_t	ifi_poll_wakeups_lowat;	/* wakeups low watermark */
	u_int32_t	ifi_poll_wakeups_hiwat;	/* wakeups high watermark */

	u_int64_t	ifi_poll_packets;	/* total # of polled packets */
	u_int32_t	ifi_poll_packets_avg;	/* average polled packets */
	u_int32_t	ifi_poll_packets_min;	/* smallest polled packets */
	u_int32_t	ifi_poll_packets_max;	/* largest polled packets */
	u_int32_t	ifi_poll_packets_lowat;	/* packets low watermark */
	u_int32_t	ifi_poll_packets_hiwat;	/* packets high watermark */

	u_int64_t	ifi_poll_bytes;		/* total # of polled bytes */
	u_int32_t	ifi_poll_bytes_avg;	/* average polled bytes */
	u_int32_t	ifi_poll_bytes_min;	/* smallest polled bytes */
	u_int32_t	ifi_poll_bytes_max;	/* largest polled bytes */
	u_int32_t	ifi_poll_bytes_lowat;	/* bytes low watermark */
	u_int32_t	ifi_poll_bytes_hiwat;	/* bytes high watermark */

	u_int32_t	ifi_poll_packets_limit;	/* max packets per poll call */
	u_int64_t	ifi_poll_interval_time;	/* poll interval (nsec) */
};

struct if_tcp_ecn_perf_stat {
	u_int64_t total_txpkts;
	u_int64_t total_rxmitpkts;
	u_int64_t total_rxpkts;
	u_int64_t total_oopkts;
	u_int64_t total_reorderpkts;
	u_int64_t rtt_avg;
	u_int64_t rtt_var;
	u_int64_t sack_episodes;
	u_int64_t rxmit_drop;
	u_int64_t rst_drop;
	u_int64_t oo_percent;
	u_int64_t reorder_percent;
	u_int64_t rxmit_percent;
};

struct if_tcp_ecn_stat {
	u_int64_t timestamp;
	u_int64_t ecn_client_setup;
	u_int64_t ecn_server_setup;
	u_int64_t ecn_client_success;
	u_int64_t ecn_server_success;
	u_int64_t ecn_peer_nosupport;
	u_int64_t ecn_syn_lost;
	u_int64_t ecn_synack_lost;
	u_int64_t ecn_recv_ce;
	u_int64_t ecn_recv_ece;
	u_int64_t ecn_conn_recv_ce;
	u_int64_t ecn_conn_recv_ece;
	u_int64_t ecn_conn_plnoce;
	u_int64_t ecn_conn_plce;
	u_int64_t ecn_conn_noplce;
	u_int64_t ecn_fallback_synloss;
	u_int64_t ecn_fallback_reorder;
	u_int64_t ecn_fallback_ce;
	u_int64_t ecn_off_conn;
	u_int64_t ecn_total_conn;
	u_int64_t ecn_fallback_droprst;
	u_int64_t ecn_fallback_droprxmt;
	struct if_tcp_ecn_perf_stat ecn_on;
	struct if_tcp_ecn_perf_stat ecn_off;
};

/*
 * Interface link status report -- includes statistics related to
 * the link layer technology sent by the driver. The driver will monitor
 * these statistics over an interval (3-4 secs) and will generate a report
 * to the network stack. This will give first-hand information about the
 * status of the first hop of the network path. The version and
 * length values should be correct for the data to be processed correctly.
 * The definitions are different for different kind of interfaces like
 * Wifi, Cellular etc,.
 */
#define IF_CELLULAR_STATUS_REPORT_VERSION_1	1
#define IF_WIFI_STATUS_REPORT_VERSION_1		1
#define IF_CELLULAR_STATUS_REPORT_CURRENT_VERSION	\
					IF_CELLULAR_STATUS_REPORT_VERSION_1
#define IF_WIFI_STATUS_REPORT_CURRENT_VERSION	IF_WIFI_STATUS_REPORT_VERSION_1
/*
 * For cellular interface --
 * There is no way to share common headers between the Baseband and
 * the kernel. Any changes to this structure will need to be communicated
 * to the Baseband team. It is better to use reserved space instead of
 * changing the size or existing fields in the structure.
 */
struct if_cellular_status_v1 {
	u_int32_t valid_bitmask; /* indicates which fields are valid */
#define IF_CELL_LINK_QUALITY_METRIC_VALID	0x1
#define IF_CELL_UL_EFFECTIVE_BANDWIDTH_VALID	0x2
#define IF_CELL_UL_MAX_BANDWIDTH_VALID		0x4
#define IF_CELL_UL_MIN_LATENCY_VALID		0x8
#define IF_CELL_UL_EFFECTIVE_LATENCY_VALID	0x10
#define IF_CELL_UL_MAX_LATENCY_VALID		0x20
#define IF_CELL_UL_RETXT_LEVEL_VALID		0x40
#define IF_CELL_UL_BYTES_LOST_VALID		0x80
#define IF_CELL_UL_MIN_QUEUE_SIZE_VALID		0x100
#define IF_CELL_UL_AVG_QUEUE_SIZE_VALID		0x200
#define IF_CELL_UL_MAX_QUEUE_SIZE_VALID		0x400
#define IF_CELL_DL_EFFECTIVE_BANDWIDTH_VALID	0x800
#define IF_CELL_DL_MAX_BANDWIDTH_VALID		0x1000
#define IF_CELL_CONFIG_INACTIVITY_TIME_VALID	0x2000
#define IF_CELL_CONFIG_BACKOFF_TIME_VALID	0x4000
#define	IF_CELL_UL_MSS_RECOMMENDED_VALID	0x8000

	u_int32_t link_quality_metric;
	u_int32_t ul_effective_bandwidth; /* Measured uplink bandwidth based on current activity (bps) */
	u_int32_t ul_max_bandwidth; /* Maximum supported uplink bandwidth (bps) */
	u_int32_t ul_min_latency; /* min expected uplink latency for first hop (ms) */
	u_int32_t ul_effective_latency; /* current expected uplink latency for first hop (ms) */
	u_int32_t ul_max_latency; /* max expected uplink latency first hop (ms) */
	u_int32_t ul_retxt_level; /* Retransmission metric */
#define IF_CELL_UL_RETXT_LEVEL_NONE	1
#define IF_CELL_UL_RETXT_LEVEL_LOW	2
#define IF_CELL_UL_RETXT_LEVEL_MEDIUM	3
#define IF_CELL_UL_RETXT_LEVEL_HIGH	4
	u_int32_t ul_bytes_lost; /* % of total bytes lost on uplink in Q10 format */
	u_int32_t ul_min_queue_size; /* minimum bytes in queue */
	u_int32_t ul_avg_queue_size; /* average bytes in queue */
	u_int32_t ul_max_queue_size; /* maximum bytes in queue */
	u_int32_t dl_effective_bandwidth; /* Measured downlink bandwidth based on current activity (bps) */
	u_int32_t dl_max_bandwidth; /* Maximum supported downlink bandwidth (bps) */
	u_int32_t config_inactivity_time; /* ms */
	u_int32_t config_backoff_time; /* new connections backoff time in ms */
#define	IF_CELL_UL_MSS_RECOMMENDED_NONE	0x0 /* Use default */
#define	IF_CELL_UL_MSS_RECOMMENDED_MEDIUM 0x1 /* 1200 byte MSS */
#define	IF_CELL_UL_MSS_RECOMMENDED_LOW	0x2 /* 512 byte MSS */
	u_int16_t mss_recommended;
	u_int16_t reserved_1;
	u_int32_t reserved_2;
	u_int64_t reserved_3;
	u_int64_t reserved_4;
	u_int64_t reserved_5;
	u_int64_t reserved_6;
} __attribute__((packed));

struct if_cellular_status {
	union {
		struct if_cellular_status_v1 if_status_v1;
	} if_cell_u;
};

/*
 * These statistics will be provided by the Wifi driver periodically.
 * After sending each report, the driver should start computing again
 * for the next report duration so that the values represent the link
 * status for one report duration.
 */

struct if_wifi_status_v1 {
	u_int32_t valid_bitmask;
#define IF_WIFI_LINK_QUALITY_METRIC_VALID	0x1
#define IF_WIFI_UL_EFFECTIVE_BANDWIDTH_VALID	0x2
#define IF_WIFI_UL_MAX_BANDWIDTH_VALID		0x4
#define IF_WIFI_UL_MIN_LATENCY_VALID		0x8
#define IF_WIFI_UL_EFFECTIVE_LATENCY_VALID	0x10
#define IF_WIFI_UL_MAX_LATENCY_VALID		0x20
#define IF_WIFI_UL_RETXT_LEVEL_VALID		0x40
#define IF_WIFI_UL_ERROR_RATE_VALID		0x80
#define IF_WIFI_UL_BYTES_LOST_VALID		0x100
#define IF_WIFI_DL_EFFECTIVE_BANDWIDTH_VALID	0x200
#define IF_WIFI_DL_MAX_BANDWIDTH_VALID		0x400
#define IF_WIFI_DL_MIN_LATENCY_VALID		0x800
#define IF_WIFI_DL_EFFECTIVE_LATENCY_VALID	0x1000
#define IF_WIFI_DL_MAX_LATENCY_VALID		0x2000
#define IF_WIFI_DL_ERROR_RATE_VALID		0x4000
#define IF_WIFI_CONFIG_FREQUENCY_VALID		0x8000
#define IF_WIFI_CONFIG_MULTICAST_RATE_VALID	0x10000
#define IF_WIFI_CONFIG_SCAN_COUNT_VALID		0x20000
#define IF_WIFI_CONFIG_SCAN_DURATION_VALID	0x40000
	u_int32_t link_quality_metric; /* link quality metric */
	u_int32_t ul_effective_bandwidth; /* Measured uplink bandwidth based on current activity (bps) */
	u_int32_t ul_max_bandwidth; /* Maximum supported uplink bandwidth (bps) */
	u_int32_t ul_min_latency; /* min expected uplink latency for first hop (ms) */
	u_int32_t ul_effective_latency; /* current expected uplink latency for first hop (ms) */
	u_int32_t ul_max_latency; /* max expected uplink latency for first hop (ms) */
	u_int32_t ul_retxt_level; /* Retransmission metric */
#define IF_WIFI_UL_RETXT_LEVEL_NONE	1
#define IF_WIFI_UL_RETXT_LEVEL_LOW	2
#define IF_WIFI_UL_RETXT_LEVEL_MEDIUM	3
#define IF_WIFI_UL_RETXT_LEVEL_HIGH	4
	u_int32_t ul_bytes_lost; /* % of total bytes lost on uplink in Q10 format */
	u_int32_t ul_error_rate; /* % of bytes dropped on uplink after many retransmissions in Q10 format */
	u_int32_t dl_effective_bandwidth; /* Measured downlink bandwidth based on current activity (bps) */
	u_int32_t dl_max_bandwidth; /* Maximum supported downlink bandwidth (bps) */
	/*
	 * The download latency values indicate the time AP may have to wait for the
	 * driver to receive the packet. These values give the range of expected latency
	 * mainly due to co-existence events and channel hopping where the interface
	 * becomes unavailable.
	 */
	u_int32_t dl_min_latency; /* min expected latency for first hop in ms */
	u_int32_t dl_effective_latency; /* current expected latency for first hop in ms */
	u_int32_t dl_max_latency; /* max expected latency for first hop in ms */
	u_int32_t dl_error_rate; /* % of CRC or other errors in Q10 format */
	u_int32_t config_frequency; /* 2.4 or 5 GHz */
#define IF_WIFI_CONFIG_FREQUENCY_2_4_GHZ	1
#define IF_WIFI_CONFIG_FREQUENCY_5_0_GHZ	2
	u_int32_t config_multicast_rate; /* bps */
	u_int32_t scan_count; /* scan count during the previous period */
	u_int32_t scan_duration; /* scan duration in ms */
	u_int64_t reserved_1;
	u_int64_t reserved_2;
	u_int64_t reserved_3;
	u_int64_t reserved_4;
} __attribute__((packed));

struct if_wifi_status {
	union {
		struct if_wifi_status_v1 if_status_v1;
	} if_wifi_u;
};

struct if_link_status {
	u_int32_t	ifsr_version;	/* version of this report */
	u_int32_t	ifsr_len;	/* length of the following struct */
	union {
		struct if_cellular_status ifsr_cell;
		struct if_wifi_status ifsr_wifi;
	} ifsr_u;
};

struct if_interface_state {
	/*
	 * The bitmask tells which of the fields
	 * to consider:
	 * - When setting, to control which fields
	 *   are being modified;
	 * - When getting, it tells which fields are set.
	 */
	u_int8_t valid_bitmask;
#define	IF_INTERFACE_STATE_RRC_STATE_VALID		0x1
#define	IF_INTERFACE_STATE_LQM_STATE_VALID		0x2
#define	IF_INTERFACE_STATE_INTERFACE_AVAILABILITY_VALID	0x4

	/*
	 * Valid only for cellular interface
	 */
	u_int8_t rrc_state;
#define	IF_INTERFACE_STATE_RRC_STATE_IDLE	0x0
#define	IF_INTERFACE_STATE_RRC_STATE_CONNECTED	0x1

	/*
	 * Values normalized to the edge of the following values
	 * that are defined on <net/if.h>:
	 *  IFNET_LQM_THRESH_BAD
	 *  IFNET_LQM_THRESH_POOR
	 *  IFNET_LQM_THRESH_GOOD
	 */
	int8_t lqm_state;

	/*
	 * Indicate if the underlying link is currently
	 * available
	 */
	u_int8_t interface_availability;
#define	IF_INTERFACE_STATE_INTERFACE_AVAILABLE		0x0
#define	IF_INTERFACE_STATE_INTERFACE_UNAVAILABLE	0x1
};

struct chain_len_stats {
	uint64_t	cls_one;
	uint64_t	cls_two;
	uint64_t	cls_three;
	uint64_t	cls_four;
	uint64_t	cls_five_or_more;
};


#pragma pack()

/*
 * Structure defining a queue for a network interface.
 */
struct	ifqueue {
	void	*ifq_head;
	void	*ifq_tail;
	int	ifq_len;
	int	ifq_maxlen;
	int	ifq_drops;
};


#define	if_mtu		if_data.ifi_mtu
#define	if_type		if_data.ifi_type
#define if_typelen	if_data.ifi_typelen
#define if_physical	if_data.ifi_physical
#define	if_addrlen	if_data.ifi_addrlen
#define	if_hdrlen	if_data.ifi_hdrlen
#define	if_preamblelen	if_data.ifi_preamblelen
#define	if_metric	if_data.ifi_metric
#define	if_baudrate	if_data.ifi_baudrate
#define	if_hwassist	if_data.ifi_hwassist
#define	if_ipackets	if_data.ifi_ipackets
#define	if_ierrors	if_data.ifi_ierrors
#define	if_opackets	if_data.ifi_opackets
#define	if_oerrors	if_data.ifi_oerrors
#define	if_collisions	if_data.ifi_collisions
#define	if_ibytes	if_data.ifi_ibytes
#define	if_obytes	if_data.ifi_obytes
#define	if_imcasts	if_data.ifi_imcasts
#define	if_omcasts	if_data.ifi_omcasts
#define	if_iqdrops	if_data.ifi_iqdrops
#define	if_noproto	if_data.ifi_noproto
#define	if_lastchange	if_data.ifi_lastchange
#define if_recvquota	if_data.ifi_recvquota
#define	if_xmitquota	if_data.ifi_xmitquota


/*
 * All of the following IF_HWASSIST_* flags are defined in kpi_inteface.h as
 * IFNET_* flags. These are redefined here as constants to avoid failures to
 * build user level programs that can not include kpi_interface.h. It is
 * important to keep this in sync with the definitions in kpi_interface.h.
 * The corresponding constant for each definition is mentioned in the comment.
 *
 * Bottom 16 bits reserved for hardware checksum
 */
#define IF_HWASSIST_CSUM_IP		0x0001	/* will csum IP, IFNET_CSUM_IP */
#define IF_HWASSIST_CSUM_TCP		0x0002	/* will csum TCP, IFNET_CSUM_TCP */
#define IF_HWASSIST_CSUM_UDP		0x0004	/* will csum UDP, IFNET_CSUM_UDP */
#define IF_HWASSIST_CSUM_IP_FRAGS	0x0008	/* will csum IP fragments, IFNET_CSUM_FRAGMENT */
#define IF_HWASSIST_CSUM_FRAGMENT	0x0010	/* will do IP fragmentation, IFNET_IP_FRAGMENT */
#define IF_HWASSIST_CSUM_TCPIPV6	0x0020	/* will csum TCPv6, IFNET_CSUM_TCPIPV6 */
#define IF_HWASSIST_CSUM_UDPIPV6	0x0040	/* will csum UDPv6, IFNET_CSUM_UDP */
#define IF_HWASSIST_CSUM_FRAGMENT_IPV6	0x0080	/* will do IPv6 fragmentation, IFNET_IPV6_FRAGMENT */
#define IF_HWASSIST_CSUM_PARTIAL	0x1000	/* simple Sum16 computation, IFNET_CSUM_PARTIAL */
#define IF_HWASSIST_CSUM_MASK		0xffff
#define IF_HWASSIST_CSUM_FLAGS(hwassist)	((hwassist) & IF_HWASSIST_CSUM_MASK)

/* VLAN support */
#define IF_HWASSIST_VLAN_TAGGING	0x00010000	/* supports VLAN tagging, IFNET_VLAN_TAGGING */
#define IF_HWASSIST_VLAN_MTU		0x00020000	/* supports VLAN MTU-sized packet (for software VLAN), IFNET_VLAN_MTU */

/* TCP Segment Offloading support */

#define IF_HWASSIST_TSO_V4		0x00200000	/* will do TCP Segment offload for IPv4, IFNET_TSO_IPV4 */
#define IF_HWASSIST_TSO_V6		0x00400000	/* will do TCP Segment offload for IPv6, IFNET_TSO_IPV6 */

#define	IFXNAMSIZ	(IFNAMSIZ + 8)	/* external name (name + unit) */

#endif /* !_NET_IF_VAR_H_ */
