import csv

with open('configs/data.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)
    with open('configs/ppin.csv', 'r') as newfile:
        newreader = csv.reader(newfile)
        for row, col, pin in newreader:
            
            for i,sign,startr,startc,endr,endc,count in reader:
                if int(count) > 0:
                    if startr == endr:
                        for j in range(int(startc), int(endc)+1):
                            with open('output.txt', 'a') as output:
                                output.write(startr+  ','  + str(j) + ',' + str(i) + '\n')
                            print(f'{startr},{j},{i}')
                    elif startc == endc :
                        for j in range(int(startr), int(endr)+1):
                            with open('output.txt', 'a') as output:
                                output.write(str(j) + ','  + startc + ',' + str(i) + '\n')
                            print(f'{j},{startc},{i}')