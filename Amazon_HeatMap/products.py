fileref = open("AmazonData.txt","r")

x = fileref.readlines()

product_lst=[]

product=""
for line in x:
	if line.split()[4]!='null':
		product=line.split()[4]
	if product not in product_lst:
		product_lst.append(product)

for prod in product_lst:
	print(prod)
	print('')

outfile = open("products.txt","w")

for prod in product_lst:
	outfile.write(prod + '\n')

outfile.close()
fileref.close()