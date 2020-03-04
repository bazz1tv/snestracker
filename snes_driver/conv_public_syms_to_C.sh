echo '#pragma once

struct TrackerApuRam
{'

tabs="\t"
declare -a sizedecl
sizedecl=('uint8_t' 'uint16_t')

startline=$(($(grep -n -m1 PUBLIC_START spc.sym | cut -f1 -d:) ))
endline=$(($(grep -n -m1 PUBLIC_END spc.sym | cut -f1 -d:) ))
#echo $startline:$endline
line=$(sed -n "$((startline)),$((startline))p" spc.sym | sed 's/00://')
padding=$((0x${line/ */}))
echo -e "${tabs}uint8_t padding[$padding];"

# Aw man, I could have used the symbol file's _sizeof_* for the sizes but
# it eluded me before writing the calculation code below. Oh well

line=$(sed -n "$((startline + 1)),$((startline + 1))p" spc.sym | sed 's/00://')
prevaddr=$((0x${line/ */}))
prevname=${line/* /}
totalsize=
while IFS= read -r line; do
  #echo "$line"
  #echo "$((0x${line/ */}))"
  #echo "${line/* /}"
  addr=$((0x${line/ */}))
  name=${line/* /}
  prevsize=$((addr - prevaddr))
	totalsize=$((totalsize + prevsize))
  echo -e "${tabs}${sizedecl[$((prevsize - 1))]} ${prevname};"
  prevname=$name
  prevaddr=$addr
done <<< $(sed -n "$((startline+2)),${endline}p" spc.sym | sed 's/00://')

printf "uint8_t padding2[0x%x];\n" $((0x10000 - totalsize - padding))
#sed -n "${startline},${startline}p" spc.sym | \
#  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/const uint16_t \2 = 0x\1,/'
#sed -n "$((startline+1)),$((endline-1))p" spc.sym | \
#  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/\t\2 = 0x\1,/'
#sed -n "${endline},${endline}p" spc.sym | \
#  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/\t\2 = 0x\1;/'

echo "} __attribute__((packed));"
