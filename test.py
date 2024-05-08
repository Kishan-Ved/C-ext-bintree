from bintree import BinTree

def int_comp(curr, new):
    if curr > new:
        return 1
    elif curr < new:
        return -1
    else:
        return 0

b = BinTree(1, 666);
print(b)

print(b.data)
print(b.key)

# Keywords can now be used. But all must be keywords or none
b.insert(data=777, comparator=int_comp, key=2)
# b = b.insert(2, 777, int_comp)
b.insert(3, 888, int_comp)
b.insert(4, 555, int_comp)
print(b.listify())
b.insert(3, 333, int_comp)
print(b.listify())
# print(b.left)
# print(b.right)

# print(str(b))

# b.left = BinTree(777)
# b.right = BinTree(888)

# print(b.listify())
