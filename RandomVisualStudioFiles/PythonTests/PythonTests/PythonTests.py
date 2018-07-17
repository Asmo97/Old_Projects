class Employee:

    num = 0
    raise_amount = 1.04

    def __init__(self, first, last, pay): #create a method (or member function) in the class which recives the instance self as the first argument
        #create instance variables (or member variables)
        self.first = first
        self.last = last
        self.pay = pay
        self.email = first + '.' + last + '@comapny.com'

        Employee.num += 1

    def fullname(self):
        print(self.first + '.' + self.last)

    def add_raise(self):
        self.pay = int(self.pay * self.raise_amount)

emp1 = Employee('ASad', 'M',50000)
emp2 = Employee('test', 'a',60000)


print(emp1.__dict__)
emp1.raise_amount = 1.05
print(emp1.__dict__)