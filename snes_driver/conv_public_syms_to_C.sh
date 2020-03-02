startline=$(($(grep -n -m1 PUBLIC_START spc.sym | cut -f1 -d:) + 1))
endline=$(($(grep -n -m1 PUBLIC_END spc.sym | cut -f1 -d:) - 1))
echo $startline:$endline
sed -n "${startline},${startline}p" spc.sym | \
  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/const uint16_t \2 = 0x\1,/'
sed -n "$((startline+1)),$((endline-1))p" spc.sym | \
  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/\t\2 = 0x\1,/'
sed -n "${endline},${endline}p" spc.sym | \
  sed 's/00:\([0-9a-fA-F]\{4\}\) \(.*\)$/\t\2 = 0x\1;/'

