num=312
num=1
num2=2
result=0
result=(num + num2)
if (result == 3):
    print("Pass add test")
else:
    print("Fail add test")
result=(num2 - num)
if (result == 1):
    print("Pass sub test")
else:
    print("Fail sub test")
result=(num * num2)
if (result == 2):
    print("Pass mul test")
else:
    print("Fail mul test")
result=(num2 / num)
if (result == 2):
    print("Pass div test")
else:
    print("Fail div test")
result=(num % num2)
if (result == 2):
    print("Pass mod test")
else:
    print("Fail mod test")
print("pass variable declaration test")
testList=[1, 2, (1 + 2)]
if (len(testList) == 3):
    print("Pass List size test")
else:
    print("Fail List size test")
if (testList[0] == 1):
    print("Pass List at test")
else:
    print("Fail List at test")
if (testList[2] == 3):
    print("Pass List at with operation test")
else:
    print("Fail List at with operation test")
positionOne=1
testListAt=["1", "L", "Words"]
if (testListAt[positionOne] == "L"):
    print("Pass Mixed List at test1")
else:
    print("Fail Mixed List at test1")
if (testListAt[(positionOne + 1)] == "Words"):
    print("Pass Mixed List at test2")
else:
    print("Fail Mixed List at test2")
while(True):
    y=0
    if (y == 10):
        while(True):
            z=0
            if (z > 5):
                break
print("Pass iteration test")
num=10
if ((num >= 0) and (num <= 5)):
    print("Fail basic test")
elif ((num > 5) and (num < 10)):
    print("Fail basic test")
elif (num != 10):
    print("Fail basic test")
else:
    print("Pass basic test")
if False:
    print("Failed real cake condition")
if True:
    print("Pass real cake condition")
if True:
    y=10
    num=(num + y)
if (num == 20):
    print("Pass inscope test")
else:
    print("Faile inscope test")
num=5
if ((num < 10) and (num > 1)):
    print("Pass and test")
else:
    print("Fail and test")
if ((num == 5) or False):
    print("Pass or test")
else:
    print("Fail or test")
userIn=int(input())
print(userIn)
def adding (num1, num2):
    result=(num1 + num2)
    return result
def checkValid (valid):
    if valid:
        result=adding(10, 10)
        return result
    else:
        return 0
print("Pass Function Creation test")
num=1
num2=2
result=adding(num, num2)
if (result == 3):
    print("Pass function call with variables")
else:
    print("Fail function call with variables")
result=adding(5, 10)
if (result == 15):
    print("Pass function call with values")
else:
    print("Fail function call with values")
result=checkValid(True)
if (result == 20):
    print("Pass nested function call 1")
else:
    print("Fail nested function call 1")
result=checkValid(False)
if (result == 0):
    print("Pass nested function call 2")
else:
    print("Fail nested function call 2")
class CTestClassC:
    def __init__ (selfish, fixedNumber):
        selfish.fixedNumber=fixedNumber
        selfish.__innerNumber=10
        selfish._list=[1, 2, 3]
        print(selfish._list)
    def Adding (selfish, num1):
        return (num1 + selfish.fixedNumber)
    def __PrivateFunction (selfish, num1):
        print("In Private Function Rightnow")
        result=selfish.Adding(num1)
        result=(result + selfish.__innerNumber)
        return result
    def TestPrivate (selfish, num1):
        result=selfish.__PrivateFunction(num1)
        return result
class CanimalandHumanC:
    def __init__ (selfish, ):
        pass
class CanimalC(CanimalandHumanC):
    def DECELERATE (selfish, speed1, x):
        print(8)
        return 8
    def __init__ (selfish, ):
        super().__init__()
        pass
class CMyVehicleC(CanimalC):
    def __init__ (selfish, speed, value):
        super().__init__()
        pass
    def DECELERATE (selfish, speed1, speed2):
        super().DECELERATE(3, 3)
        print(8)
        return 8
print("Class Created Successfully")
testcase = CTestClassC(5)
print("Pass Class declaration test")
num=3
result=testcase.Adding(num)
if (result == 8):
    print("Pass public function test")
else:
    print("Fail public function test")
result=testcase.TestPrivate(num)
if (result == 18):
    print("Pass private function test")
else:
    print("Fail private function test")
eee = CMyVehicleC(3, 10)
eee.DECELERATE(3, 3)
