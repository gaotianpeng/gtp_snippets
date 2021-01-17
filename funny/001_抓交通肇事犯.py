def GetCarNumber():
    flag = False
    for i in range(10):
        if flag:
            break;
        for j in range(10):
            if flag:
                break;
            if i != j:
                k = 1000 * i + 100 * i + 10 * j + j
                for temp in range(31, 100):
                    if temp * temp == k:
                        flag = True
                        return k

print(GetCarNumber())