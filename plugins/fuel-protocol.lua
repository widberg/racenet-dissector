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

p_fuel.fields = {f_size, f_index, f_hash, f_decompressed_size, f_data, f_decompressed_data}

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
        -- Dissect the uncompressed/decompressed data
    end
end

local udp_port = DissectorTable.get("udp.port")
original_xlln_dissector = udp_port:get_dissector(2000)
udp_port:add(2000, p_fuel)
