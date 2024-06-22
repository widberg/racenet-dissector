local plugins_path = debug.getinfo(1).source:match("@?(.*[/\\])")
package.cpath = package.cpath .. ";" .. plugins_path .. "libppmdlua/libppmdlua.dll;" .. plugins_path ..
                    "libppmdlua/libppmdlua.so"
local ppmd_status, ppmd_module = pcall(require, 'libppmdlua')
ppmd = ppmd_status and ppmd_module or nil

local p_fuel = Proto("FUEL", "FUEL Protocol");

local f_size = ProtoField.uint16("fuel.size", "size", base.HEX)
local f_index = ProtoField.uint8("fuel.index", "index", base.HEX)
local f_hash = ProtoField.uint16("fuel.hash", "hash", base.HEX)
local f_decompressed_size = ProtoField.uint16("fuel.decompressed_size", "decompressedSize", base.HEX)
local f_data = ProtoField.bytes("fuel.data", "data", base.NONE)
local f_decompressed_data = ProtoField.bytes("fuel.decompressed_data", "decompressedData", base.NONE)
local f_packet_index = ProtoField.uint8("fuel.message_index", "packetIndex", base.HEX)
local f_message_size = ProtoField.uint16("fuel.message_size", "messageSize", base.HEX)
local f_message_type = ProtoField.uint8("fuel.message_type", "messageType", base.HEX)
local f_unknown0 = ProtoField.string("fuel.unknown0", "unknown0", base.ASCII)
local f_unknown1 = ProtoField.string("fuel.unknown1", "unknown1", base.ASCII)
local f_hostname = ProtoField.string("fuel.hostname", "hostname", base.ASCII)
local f_racenet = ProtoField.string("fuel.racenet", "racenet", base.ASCII)
local f_init_data = ProtoField.bytes("fuel.init_data", "initData", base.NONE)
local f_syn_ack_byte = ProtoField.uint8("fuel.syn_ack_byte", "synAckByte", base.HEX)
local f_ack_byte = ProtoField.uint8("fuel.ack_byte", "ackByte", base.HEX)
local f_update_type = ProtoField.uint32("fuel.update_type", "updateType", base.HEX)
local f_update_data = ProtoField.bytes("fuel.update_data", "updateData", base.NONE)
local f_update_name = ProtoField.string("fuel.update_name", "updateName", base.ASCII)
local f_disconnected_index = ProtoField.uint32("fuel.disconnected_index", "disconnectedIndex", base.HEX)

p_fuel.fields = {f_size, f_index, f_hash, f_decompressed_size, f_data, f_decompressed_data, f_packet_index,
                 f_message_size, f_message_type, f_unknown0, f_unknown1, f_hostname, f_racenet, f_syn_ack_byte,
                 f_ack_byte, f_init_data, f_update_type, f_update_data, f_update_name, f_disconnected_index}

local title_data = Field.new("xlln.title_data")
local original_xlln_dissector

function p_fuel.dissector(buffer, pinfo, tree)
    original_xlln_dissector:call(buffer, pinfo, tree)
    if title_data() then
        pinfo.cols.protocol = p_fuel.name
        buffer = title_data().range()
        local subtree = tree:add(p_fuel, buffer, "FUEL Protocol Data")
        local size = buffer(0, 2):le_uint()
        subtree:add_le(f_size, buffer(0, 2))
        subtree:add_le(f_index, buffer(2, 1))
        subtree:add_le(f_hash, buffer(3, 2))
        local decompressed_size = buffer(5, 2):le_uint()
        subtree:add_le(f_decompressed_size, buffer(5, 2))
        local data_size = size - 5
        subtree:add_le(f_data, buffer(7, data_size))
        buffer = buffer(7, data_size):tvb()
        if decompressed_size >= 24 then
            if ppmd then
                local data = buffer:raw()
                local decompressed_data = ppmd.decompress(data, decompressed_size)
                if decompressed_data then
                    local decompressed_byte_array = ByteArray.new(decompressed_data, true)
                    buffer = decompressed_byte_array:tvb("decompressedData")
                    subtree:add(f_decompressed_data, buffer())
                end
            else
                return
            end
        end
        subtree:add_le(f_packet_index, buffer(0, 1))
        buffer = buffer(1):tvb()
        while buffer:len() ~= 0 do
            local message_size = buffer(0, 2):le_uint()
            local message = subtree:add(p_fuel, buffer(0, 2 + message_size), "FUEL Message")
            message:add_le(f_message_size, buffer(0, 2))
            local message_type = buffer(2, 1):le_uint()
            message:add_le(f_message_type, buffer(2, 1)):append_text(" (" .. get_message_type_name(message_type) .. ")")
            local message_buffer = buffer(3, message_size - 1):tvb()
            if message_type == INIT then
                message:add_le(f_init_data, message_buffer(0, 8))
            elseif message_type == ACK then
                message:add_le(f_ack_byte, message_buffer(0, 1))
            elseif message_type == SYN_ACK then
                message:add(f_syn_ack_byte, message_buffer(0, 1))
            elseif message_type == SYN then
                message:add(f_unknown0, message_buffer(0, 16))
                message:add(f_unknown1, message_buffer(16, 16))
                message:add(f_hostname, message_buffer(32, 16))
                message:add(f_racenet, message_buffer(48, 16))
            elseif message_type == UPDATE then
                local update_type = message_buffer(0, 4):le_uint()
                message:add_le(f_update_type, message_buffer(0, 4)):append_text(" (" ..
                                                                                    get_update_type_name(update_type) ..
                                                                                    ")")
                if update_type == NAME then
                    -- Complicated
                end
            elseif message_type == DISCONNECT then
                -- Do nothing
            elseif message_type == DISCONNECTED then
                message:add_le(f_disconnected_index, message_buffer(0, 4))
            end
            buffer = buffer(2 + message_size):tvb()
        end
    end
end

-- FUEL MESSAGE TYPE
INIT = 0x01
ACK = 0x02
SYN_ACK = 0x03
SYN = 0x04
UPDATE = 0x05
DISCONNECT = 0x07
DISCONNECTED = 0x08

function get_message_type_name(message_type)
    if message_type == INIT then
        return "INIT"
    elseif message_type == ACK then
        return "ACK"
    elseif message_type == SYN_ACK then
        return "SYN_ACK"
    elseif message_type == SYN then
        return "SYN"
    elseif message_type == UPDATE then
        return "UPDATE"
    elseif message_type == DISCONNECT then
        return "DISCONNECT"
    elseif message_type == DISCONNECTED then
        return "DISCONNECTED"
    end

    return "UNKNOWN"
end

-- FUEL UPDATE_MESSAGE_TYPE
NAME = 0x00000004

function get_update_type_name(update_type)
    if update_type == NAME then
        return "NAME"
    end

    return "UNKNOWN"
end

-- FUEL OR_ACTION
OR_NO_ACTION = 0x00
OR_ACTION_ONGRID = 0x01
OR_ACTION_QUITONLINERACE = 0x02
OR_ACTION_QUITONLINELOBBY = 0x03

function get_or_action_name(or_action)
    if or_action == OR_NO_ACTION then
        return "OR_NO_ACTION"
    elseif or_action == OR_ACTION_ONGRID then
        return "OR_ACTION_ONGRID"
    elseif or_action == OR_ACTION_QUITONLINERACE then
        return "OR_ACTION_QUITONLINERACE"
    elseif or_action == OR_ACTION_QUITONLINELOBBY then
        return "OR_ACTION_QUITONLINELOBBY"
    end

    return "UNKNOWN"
end

-- FUEL ORACESTATUS 
ORACESTATUS_WAITINGFORPLAYERS = 0x00
ORACESTATUS_321GO = 0x01
ORACESTATUS_INRACE = 0x02
ORACESTATUS_1STARRIVED = 0x03
ORACESTATUS_WAITING_END_RACE = 0x04
ORACESTATUS_FINAL_RANKING = 0x05
ORACESTATUS_RACE_OVER = 0x06

function get_oracestatus_name(oracestatus)
    if oracestatus == ORACESTATUS_WAITINGFORPLAYERS then
        return "ORACESTATUS_WAITINGFORPLAYERS"
    elseif oracestatus == ORACESTATUS_321GO then
        return "ORACESTATUS_321GO"
    elseif oracestatus == ORACESTATUS_INRACE then
        return "ORACESTATUS_INRACE"
    elseif oracestatus == ORACESTATUS_1STARRIVED then
        return "ORACESTATUS_1STARRIVED"
    elseif oracestatus == ORACESTATUS_WAITING_END_RACE then
        return "ORACESTATUS_WAITING_END_RACE"
    elseif oracestatus == ORACESTATUS_FINAL_RANKING then
        return "ORACESTATUS_FINAL_RANKING"
    elseif oracestatus == ORACESTATUS_RACE_OVER then
        return "ORACESTATUS_RACE_OVER"
    end

    return "UNKNOWN"
end

local udp_port = DissectorTable.get("udp.port")
original_xlln_dissector = udp_port:get_dissector(3000)
udp_port:add(3000, p_fuel)
