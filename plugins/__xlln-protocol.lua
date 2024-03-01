p_xlln = Proto("XLLN", "XLLN Protocol")

-- SOCKADDR_STORAGE
f_ss_family = ProtoField.uint16("xlln.ss_family", "ss_family", base.HEX)
f___ss_pad1 = ProtoField.bytes("xlln.__ss_pad1", "__ss_pad1", base.NONE)
f___ss_align = ProtoField.uint64("xlln.__ss_align", "__ss_align", base.HEX)
f___ss_pad2 = ProtoField.bytes("xlln.__ss_pad2", "__ss_pad2", base.NONE)

-- NET_USER_PACKET
f_instance_id = ProtoField.uint32("xlln.instance_id", "instanceId", base.HEX)
f_port_base_hbo = ProtoField.uint16("xlln.port_base_hbo", "portBaseHBO", base.HEX)
f_socket_internal_port_hbo = ProtoField.uint16("xlln.socket_internal_port_hbo", "socketInternalPortHBO", base.HEX)
f_socket_internal_port_offset_hbo = ProtoField.uint16("xlln.socket_internal_port_offset_hbo",
    "socketInternalPortOffsetHBO", base.HEX)
f_instance_id_consume_remaining = ProtoField.uint32("xlln.instance_id_consume_remaining", "instanceIdConsumeRemaining",
    base.HEX)

-- XUSER_CONTEXT
f_dw_context_id = ProtoField.uint32("xlln.dw_context_id", "dwContextId", base.HEX)
f_dw_value = ProtoField.uint32("xlln.dw_value", "dwValue", base.HEX)

-- XUSER_PROPERTY_SERIALISED
f_property_id = ProtoField.uint32("xlln.property_id", "propertyId", base.HEX)
f_type = ProtoField.uint8("xlln.type", "type", base.HEX)
f_n_data = ProtoField.uint32("xlln.n_data", "nData", base.HEX)
f_i64_data = ProtoField.int64("xlln.i64_data", "i64Data", base.DEC)
f_dbl_data = ProtoField.double("xlln.dbl_data", "dblData", base.HEX)
f_string_cb_data = ProtoField.uint32("xlln.string_cb_data", "string.cbData", base.HEX)
f_string_pwsz_data = ProtoField.string("xlln.string_pwsz_data", "string.pwszData", base.UNICODE)
f_data = ProtoField.float("xlln.f_data", "fData", base.HEX)
f_binary_cb_data = ProtoField.uint32("xlln.binary_cb_data", "binary.cbData", base.HEX)
f_binary_pb_data = ProtoField.bytes("xlln.binary_pb_data", "binary.pbData", base.NONE)
f_ft_data = ProtoField.uint64("xlln.ft_data", "ftData", base.HEX)

-- LIVE_SESSION
-- xuid
-- session_type
f_session_flags = ProtoField.uint32("xlln.session_flags", "sessionFlags", base.HEX)
f_xnkid = ProtoField.bytes("xlln.xnkid", "xnkid", base.NONE)
f_xnkey = ProtoField.bytes("xlln.xnkey", "xnkey", base.NONE)
f_slots_public_max_count = ProtoField.uint32("xlln.slots_public_max_count", "slotsPublicMaxCount", base.HEX)
f_slots_public_filled_count = ProtoField.uint32("xlln.slotsPublicFilledCount", "slotsPublicFilledCount", base.HEX)
f_slots_private_max_count = ProtoField.uint32("xlln.slotsPrivateMaxCount", "slotsPrivateMaxCount", base.HEX)
f_slots_private_filled_count = ProtoField.uint32("xlln.slotsPrivateFilledCount", "slotsPrivateFilledCount", base.HEX)
f_contexts_count = ProtoField.uint32("xlln.contextsCount", "contextsCount", base.HEX)
f_properties_count = ProtoField.uint32("xlln.propertiesCount", "propertiesCount", base.HEX)
-- XUSER_CONTEXT *pContexts
-- XUSER_PROPERTY *pProperties

-- PACKETS
f_packet_type = ProtoField.uint8("xlln.packet_type", "packetType", base.HEX)

-- TITLE
f_title_data = ProtoField.bytes("xlln.title_data", "titleData", base.NONE)

-- PACKET_FORWARDED
-- SOCKADDR_STORAGE origin_sock_addr
-- NET_USER_PACKET netter

-- CUSTOM_OTHER
-- TODO

-- UNKNOWN_USER
-- NET_USER_PACKET netter

-- LIVE_OVER_LAN_UNADVERTISE
f_session_type = ProtoField.uint8("xlln.session_type", "sessionType", base.HEX)
f_xuid = ProtoField.uint64("xlln.xuid", "xuid", base.HEX)

-- HUB_REQUEST
-- xlln_version
-- instance_id
-- title_id
f_title_version = ProtoField.uint32("xlln.title_version", "titleVersion", base.HEX)
f_port_base_hbo32 = ProtoField.uint32("xlln.port_base_hbo32", "portBaseHBO", base.HEX)

-- HUB_REPLY
f_is_hub_server = ProtoField.bool("xlln.is_hub_server", "isHubServer", 8, nil, 0x01)
f_xlln_version = ProtoField.uint32("xlln.xlln_version", "xllnVersion", base.HEX)
f_recommended_instance_id = ProtoField.uint32("xlln.recommended_instance_id", "recommendedInstanceId", base.HEX)

-- QOS_REQUEST
f_qos_lookup_id = ProtoField.uint32("xlln.qos_lookup_id", "qosLookupId", base.HEX)
f_session_id = ProtoField.uint32("xlln.session_id", "sessionId", base.HEX)
f_probe_id = ProtoField.uint32("xlln.probe_id", "probeId", base.HEX)
-- instance_id

-- QOS_RESPONSE
-- qos_lookup_id
-- session_id
-- probe_id
-- instance_id
f_enabled = ProtoField.bool("xlln.enabled", "enabled", 8, nil, 0x01)
f_size_data = ProtoField.uint16("xlln.size_data", "sizeData", base.HEX)

-- DIRECT_IP_REQUEST
f_join_request_signature = ProtoField.uint32("xlln.join_request_signature", "joinRequestSignature", base.HEX)
f_password_sha256 = ProtoField.bytes("xlln.password_sha256", "passwordSha256", base.NONE)

-- DIRECT_IP_RESPONSE
-- join_request_signature
-- instance_id
f_title_id = ProtoField.uint32("xlln.title_id", "titleId", base.HEX)

p_xlln.fields = {f_ss_family, f___ss_pad1, f___ss_align, f___ss_pad2, f_instance_id, f_port_base_hbo,
                 f_socket_internal_port_hbo, f_socket_internal_port_offset_hbo, f_instance_id_consume_remaining,
                 f_dw_context_id, f_dw_value, f_property_id, f_type, f_n_data, f_i64_data, f_dbl_data, f_string_cb_data,
                 f_string_pwsz_data, f_data, f_binary_cb_data, f_binary_pb_data, f_ft_data, f_session_flags, f_xnkid,
                 f_xnkey, f_slots_public_max_count, f_slots_public_filled_count, f_slots_private_max_count,
                 f_slots_private_filled_count, f_contexts_count, f_properties_count, f_packet_type, f_title_data,
                 f_session_type, f_xuid, f_title_version, f_port_base_hbo32, f_is_hub_server, f_xlln_version,
                 f_recommended_instance_id, f_qos_lookup_id, f_session_id, f_probe_id, f_enabled, f_size_data,
                 f_join_request_signature, f_password_sha256, f_title_id}

function p_xlln.dissector(buffer, pinfo, tree)
    while buffer:len() ~= 0 do
        pinfo.cols.protocol = p_xlln.name

        local subtree = tree:add(p_xlln, buffer(), "XLLN Protocol Data")

        local packet_type = buffer(0, 1):le_uint()
        local packet_type_name = get_packet_type_name(packet_type)
        subtree:add_le(f_packet_type, buffer(0, 1)):append_text(" (" .. packet_type_name .. ")")
        buffer = buffer(1):tvb()

        if packet_type == TITLE_BROADCAST_PACKET or packet_type == TITLE_PACKET then
            subtree:add_le(f_title_data, buffer())
            break
        elseif packet_type == PACKET_FORWARDED then
            local origin_sock_addr = subtree:add(p_xlln, buffer(0, 128), "originSockAddr")
            buffer = deserialize_sockaddr_storage(buffer, pinfo, origin_sock_addr)
            local netter = subtree:add(p_xlln, buffer(0, 14), "netter")
            buffer = deserialize_net_user_packet(buffer, pinfo, netter)
        elseif packet_type == CUSTOM_OTHER then
            -- TODO
            break
        elseif packet_type == UNKNOWN_USER_ASK or packet_type == UNKNOWN_USER_REPLY then
            local netter = subtree:add(p_xlln, buffer(0, 14), "netter")
            buffer = deserialize_net_user_packet(buffer, pinfo, netter)
        elseif packet_type == LIVE_OVER_LAN_UNADVERTISE then
            local session_type = buffer(0, 1):le_uint()
            subtree:add_le(f_session_type, buffer(0, 1)):append_text(" (" ..
                                                                         get_xlln_liveoverlan_session_type_name(
                    session_type) .. ")")
            subtree:add_le(f_xuid, buffer(1, 8))
            buffer = buffer(9):tvb()
        elseif packet_type == LIVE_OVER_LAN_ADVERTISE then
            local live_session = subtree:add(p_xlln, buffer, "live_session")
            buffer = live_over_lan_deserialize_live_session(buffer, pinfo, live_session)
        elseif packet_type == HUB_REQUEST then
            subtree:add_le(f_xlln_version, buffer(0, 4))
            subtree:add_le(f_instance_id, buffer(4, 4))
            local title_id = buffer(8, 4):le_uint()
            subtree:add_le(f_title_id, buffer(8, 4)):append_text(" (" .. get_title_id_name(title_id) .. ")")
            subtree:add_le(f_title_version, buffer(12, 4))
            subtree:add_le(f_port_base_hbo32, buffer(16, 4))
            buffer = buffer(20):tvb()
        elseif packet_type == HUB_REPLY then
            subtree:add_le(f_is_hub_server, buffer(0, 1))
            subtree:add_le(f_xlln_version, buffer(1, 4))
            subtree:add_le(f_recommended_instance_id, buffer(5, 4))
            buffer = buffer(9):tvb()
        elseif packet_type == QOS_REQUEST then
            subtree:add_le(f_qos_lookup_id, buffer(0, 4))
            subtree:add_le(f_session_id, buffer(4, 4))
            subtree:add_le(f_probe_id, buffer(8, 4))
            subtree:add_le(f_instance_id, buffer(12, 4))
            buffer = buffer(16):tvb()
        elseif packet_type == QOS_RESPONSE then
            subtree:add_le(f_qos_lookup_id, buffer(0, 4))
            subtree:add_le(f_session_id, buffer(4, 4))
            subtree:add_le(f_probe_id, buffer(8, 4))
            subtree:add_le(f_instance_id, buffer(12, 4))
            subtree:add_le(f_enabled, buffer(16, 1))
            subtree:add_le(f_size_data, buffer(17, 2))
            buffer = buffer(19):tvb()
        elseif packet_type == DIRECT_IP_REQUEST then
            subtree:add_le(f_join_request_signature, buffer(0, 4))
            subtree:add_le(f_password_sha256, buffer(4, 32))
            buffer = buffer(36):tvb()
        elseif packet_type == DIRECT_IP_RESPONSE then
            subtree:add_le(f_join_request_signature, buffer(0, 4))
            subtree:add_le(f_instance_id, buffer(4, 4))
            local title_id = buffer(8, 4):le_uint()
            subtree:add_le(f_title_id, buffer(8, 4)):append_text(" (" .. get_title_id_name(title_id) .. ")")
            buffer = buffer(12):tvb()
            local live_session = subtree:add(p_xlln, buffer(), "live_session")
            buffer = live_over_lan_deserialize_live_session(buffer, pinfo, live_session)
        else
            break
        end
    end
end

function deserialize_sockaddr_storage(buffer, pinfo, tree)
    tree:add_le(f_ss_family, buffer(0, 2))
    tree:add_le(f___ss_pad1, buffer(2, 6))
    tree:add_le(f___ss_align, buffer(8, 8))
    tree:add_le(f___ss_pad2, buffer(16, 112))
    return buffer(128):tvb()
end

function deserialize_net_user_packet(buffer, pinfo, tree)
    tree:add_le(f_instance_id, buffer(0, 4))
    tree:add_le(f_port_base_hbo, buffer(4, 2))
    tree:add_le(f_socket_internal_port_hbo, buffer(6, 2))
    tree:add_le(f_socket_internal_port_offset_hbo, buffer(8, 2))
    tree:add_le(f_instance_id_consume_remaining, buffer(10, 4))
    return buffer(14):tvb()
end

function live_over_lan_deserialize_live_session(buffer, pinfo, tree)
    tree:add_le(f_xuid, buffer(0, 8))
    local session_type = buffer(8, 1):le_uint()
    tree:add_le(f_session_type, buffer(8, 1)):append_text(
        " (" .. get_xlln_liveoverlan_session_type_name(session_type) .. ")")
    tree:add_le(f_session_flags, buffer(9, 4))
    tree:add_le(f_xnkid, buffer(13, 8))
    tree:add_le(f_xnkey, buffer(21, 16))
    tree:add_le(f_slots_public_max_count, buffer(37, 4))
    tree:add_le(f_slots_public_filled_count, buffer(41, 4))
    tree:add_le(f_slots_private_max_count, buffer(45, 4))
    tree:add_le(f_slots_private_filled_count, buffer(49, 4))
    local contexts_count = buffer(53, 4):le_uint()
    tree:add_le(f_contexts_count, buffer(53, 4))
    local properties_count = buffer(57, 4):le_uint()
    tree:add_le(f_properties_count, buffer(57, 4))
    buffer = buffer(61):tvb()
    if contexts_count ~= 0 then
        local contexts = tree:add(p_xlln, buffer(0, 8 * contexts_count), "contexts")
        for i = 0, contexts_count - 1 do
            local context = contexts:add(p_xlln, buffer(0 + i * 8, 8), "context")
            context:add_le(f_dw_context_id, buffer(0 + i * 8, 4))
            context:add_le(f_dw_value, buffer(4 + i * 8, 4))
        end
        buffer = buffer(8 * contexts_count):tvb()
    end
    if properties_count ~= 0 then
        local properties = tree:add(p_xlln, buffer(), "properties")
        for i = 0, properties_count - 1 do
            local property = properties:add(p_xlln, buffer(), "property")
            local property_id = buffer(0, 4):le_uint()
            property:add_le(f_property_id, buffer(0, 4)):append_text(" (" .. get_property_id_name(property_id) .. ")")
            local type = buffer(4, 1):le_uint()
            property:add_le(f_type, buffer(4, 1)):append_text(" (" .. get_property_type_name(type) .. ")")
            buffer = buffer(5):tvb()
            if type == XUSER_DATA_TYPE_CONTEXT or type == XUSER_DATA_TYPE_INT32 then
                property:add_le(f_n_data, buffer(0, 4))
                buffer = buffer(4):tvb()
            elseif type == XUSER_DATA_TYPE_INT64 then
                property:add_le(f_i64_data, buffer(0, 8))
                buffer = buffer(8):tvb()
            elseif type == XUSER_DATA_TYPE_DOUBLE then
                property:add_le(f_dbl_data, buffer(0, 8))
                buffer = buffer(8):tvb()
            elseif type == XUSER_DATA_TYPE_UNICODE then
                local size = buffer(0, 4):le_uint()
                property:add_le(f_string_cb_data, buffer(0, 4))
                local data = buffer(4, size):le_ustring()
                property:add_le(f_string_pwsz_data, buffer(4, size), data)
                buffer = buffer(4 + size):tvb()
            elseif type == XUSER_DATA_TYPE_FLOAT then
                property:add_le(f_data, buffer(0, 4))
                buffer = buffer(4):tvb()
            elseif type == XUSER_DATA_TYPE_BINARY then
                local size = buffer(0, 4):le_uint()
                property:add_le(f_binary_cb_data, buffer(0, 4))
                property:add_le(f_binary_pb_data, buffer(4, size))
                buffer = buffer(4 + size):tvb()
            elseif type == XUSER_DATA_TYPE_DATETIME then
                property:add_le(f_ft_data, buffer(0, 8))
                buffer = buffer(8):tvb()
            elseif type == XUSER_DATA_TYPE_NULL then
                -- nothing to do
            end
        end
    end

    return buffer
end

-- TITLE_ID
TITLE_ID_FUEL = 0x434d082f

function get_title_id_name(title_id)
    if title_id == TITLE_ID_FUEL then
        return "TITLE_ID_FUEL"
    end

    return "UNKNOWN"
end

-- XLLN_LIVEOVERLAN_SESSION_TYPE
XLLN_LIVEOVERLAN_SESSION_TYPE_XLOCATOR = 0
XLLN_LIVEOVERLAN_SESSION_TYPE_XSESSION = 1

function get_xlln_liveoverlan_session_type_name(session_type)
    if session_type == XLLN_LIVEOVERLAN_SESSION_TYPE_XLOCATOR then
        return "XLLN_LIVEOVERLAN_SESSION_TYPE_XLOCATOR"
    elseif session_type == XLLN_LIVEOVERLAN_SESSION_TYPE_XSESSION then
        return "XLLN_LIVEOVERLAN_SESSION_TYPE_XSESSION"
    end

    return "UNKNOWN"
end

-- XUSER_PROPERTY
XUSER_PROPERTY_GAMERHOSTNAME = 0x40008109

function get_property_id_name(property_id)
    if property_id == XUSER_PROPERTY_GAMERHOSTNAME then
        return "XUSER_PROPERTY_GAMERHOSTNAME"
    end

    return "UNKNOWN"
end

-- XUSER_DATA_TYPE
XUSER_DATA_TYPE_CONTEXT = 0x00
XUSER_DATA_TYPE_INT32 = 0x01
XUSER_DATA_TYPE_INT64 = 0x02
XUSER_DATA_TYPE_DOUBLE = 0x03
XUSER_DATA_TYPE_UNICODE = 0x04
XUSER_DATA_TYPE_FLOAT = 0x05
XUSER_DATA_TYPE_BINARY = 0x06
XUSER_DATA_TYPE_DATETIME = 0x07
XUSER_DATA_TYPE_NULL = 0x08

function get_property_type_name(property_type)
    if property_type == XUSER_DATA_TYPE_CONTEXT then
        return "XUSER_DATA_TYPE_CONTEXT"
    elseif property_type == XUSER_DATA_TYPE_INT32 then
        return "XUSER_DATA_TYPE_INT32"
    elseif property_type == XUSER_DATA_TYPE_INT64 then
        return "XUSER_DATA_TYPE_INT64"
    elseif property_type == XUSER_DATA_TYPE_DOUBLE then
        return "XUSER_DATA_TYPE_DOUBLE"
    elseif property_type == XUSER_DATA_TYPE_UNICODE then
        return "XUSER_DATA_TYPE_UNICODE"
    elseif property_type == XUSER_DATA_TYPE_FLOAT then
        return "XUSER_DATA_TYPE_FLOAT"
    elseif property_type == XUSER_DATA_TYPE_BINARY then
        return "XUSER_DATA_TYPE_BINARY"
    elseif property_type == XUSER_DATA_TYPE_DATETIME then
        return "XUSER_DATA_TYPE_DATETIME"
    elseif property_type == XUSER_DATA_TYPE_NULL then
        return "XUSER_DATA_TYPE_NULL"
    end

    return "UNKNOWN"
end

-- XLLN PACKET TYPE
UNKNOWN = 0x00
TITLE_PACKET = 0x01
TITLE_BROADCAST_PACKET = 0x02
PACKET_FORWARDED = 0x03
UNKNOWN_USER_ASK = 0x04
UNKNOWN_USER_REPLY = 0x05
CUSTOM_OTHER = 0x06
LIVE_OVER_LAN_ADVERTISE = 0x07
LIVE_OVER_LAN_UNADVERTISE = 0x08
HUB_REQUEST = 0x09
HUB_REPLY = 0x0A
QOS_REQUEST = 0x0B
QOS_RESPONSE = 0x0C
HUB_OUT_OF_BAND = 0x0D
HUB_RELAY = 0x0E
DIRECT_IP_REQUEST = 0x0F
DIRECT_IP_RESPONSE = 0x10

function get_packet_type_name(packet_type)
    if packet_type == TITLE_PACKET then
        return "TITLE_PACKET"
    elseif packet_type == TITLE_BROADCAST_PACKET then
        return "TITLE_BROADCAST_PACKET"
    elseif packet_type == PACKET_FORWARDED then
        return "PACKET_FORWARDED"
    elseif packet_type == UNKNOWN_USER_ASK then
        return "UNKNOWN_USER_ASK"
    elseif packet_type == UNKNOWN_USER_REPLY then
        return "UNKNOWN_USER_REPLY"
    elseif packet_type == CUSTOM_OTHER then
        return "CUSTOM_OTHER"
    elseif packet_type == LIVE_OVER_LAN_ADVERTISE then
        return "LIVE_OVER_LAN_ADVERTISE"
    elseif packet_type == LIVE_OVER_LAN_UNADVERTISE then
        return "LIVE_OVER_LAN_UNADVERTISE"
    elseif packet_type == HUB_REQUEST then
        return "HUB_REQUEST"
    elseif packet_type == HUB_REPLY then
        return "HUB_REPLY"
    elseif packet_type == QOS_REQUEST then
        return "QOS_REQUEST"
    elseif packet_type == QOS_RESPONSE then
        return "QOS_RESPONSE"
    elseif packet_type == HUB_OUT_OF_BAND then
        return "HUB_OUT_OF_BAND"
    elseif packet_type == HUB_RELAY then
        return "HUB_RELAY"
    elseif packet_type == DIRECT_IP_REQUEST then
        return "DIRECT_IP_REQUEST"
    elseif packet_type == DIRECT_IP_RESPONSE then
        return "DIRECT_IP_RESPONSE"
    end

    return "UNKNOWN"
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(2000, p_xlln)
