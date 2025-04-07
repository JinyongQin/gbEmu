def compare_files(file1_path, file2_path):
    with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
        line_number = 1
        for line1, line2 in zip(file1, file2):
            if line1 != line2:
                print(f"第一个不同的行号: {line_number}")
                print(f"文件1内容: {line1.strip()}")
                print(f"文件2内容: {line2.strip()}")
                return
            line_number += 1
        
        # 检查文件长度是否不同
        if any(line for line in file1):
            print(f"文件1在行号 {line_number} 后还有内容，但文件2已结束")
        elif any(line for line in file2):
            print(f"文件2在行号 {line_number} 后还有内容，但文件1已结束")
        else:
            print("两个文件内容完全相同")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("用法: python cmp.py 文件1 文件2")
        sys.exit(1)
    
    file1_path = sys.argv[1]
    file2_path = sys.argv[2]
    
    compare_files(file1_path, file2_path)
