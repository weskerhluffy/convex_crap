import math
from collections import namedtuple  
import sys

# XXX: http://www.w3resource.com/python-exercises/math/python-math-exercise-79.php
#@profile
def calcula_caca(p1,p2):
    dif_x=p2[0]-p1[0]
    dif_y=p2[1]-p1[1]
#    distance=math.sqrt(dif_x*dif_x+dif_y*dif_y)
    distance=dif_x*dif_x+dif_y*dif_y
    anogulo=math.atan2(dif_y,dif_x)
# XXX: https://stackoverflow.com/questions/9875964/python-converting-radians-to-degrees
#    anogulo=anogulo* 180 / math.pi
    return anogulo, distance, p2

#@profile
def main():  
    num_cacasos=int(next(sys.stdin).strip())
#    print("contr las %s"%num_cacasos)
    putos=[]
    while(sys.stdin):
        linea=next(sys.stdin,None)
#        print("linea caca %s"%linea)
        if(linea is None or " " not in linea):
            if(putos):
#                print("Points on husll: {}".format(putos))
                min_puto=min(putos)
#                print("hold %s, %s"%min_puto)
                putos_polares=[]
                for puto in putos:
                    if(puto==min_puto):
                        continue
                    ano,dist,put=calcula_caca(min_puto,puto)
                    putos_polares.append((ano,dist,put))
                max_puto=max(putos_polares,key=lambda x:(x[1],x[0],x[2]))
#                print("max puto %s %s %s"%max_puto)
#                putos_polares.sort(key=lambda x:(x[0],x[1]*(-1 if (x[0],x[1])>(max_puto[0],max_puto[1]) else 1)))
                putos_polares.sort()
                putos_polares_tam=len(putos_polares)
                for idx_p,puto_polar in enumerate(putos_polares):
#                    print("comparando %s con %s"%(max_puto,puto_polar))
                    if(max_puto==puto_polar):
                        break
#                print("idx_puto %s %s"%(idx_p,len(putos_polares)))
                putos_polares=putos_polares[:idx_p+1]+(sorted(putos_polares[idx_p+1:],key=lambda x:(x[0],x[1]*-1)) if (idx_p+1<putos_polares_tam) else [])
#                print("mierdeas : {}".format(putos_polares))
#                print("putos polares : {}".format([min_puto]+[x[2] for x in putos_polares]))
                print("%s"%(len(putos_polares)+1))
                print("%s %s"%min_puto)
                for puto in putos_polares:
                    print("%s %s"%(puto[2][0],puto[2][1]))
                
            putos=[]
#            print("reseteado ch")
            if linea is None:
                break
            else:
                continue
        caca_x,caca_y,mierda=(x for x in linea.strip().split(" "))
#        print("saint seiya {} {}".format(caca_x,caca_y))
        if(mierda=='Y'):
            putos.append((int(caca_x), int(caca_y)))


if __name__ == '__main__':  
    main()
