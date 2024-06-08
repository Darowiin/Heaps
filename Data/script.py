import pandas as pd

def parse_raw_data(raw_data):
    binomial_data = {
        'Size': [],
        'Operation': [],
        'Data Structure': [],
        'Time (ms)': []
    }

    fibonacci_data = {
        'Size': [],
        'Operation': [],
        'Data Structure': [],
        'Time (ms)': []
    }

    current_size = None
    current_operation = None

    for item in raw_data:
        if 'SIZE =' in item:
            current_size = int(item.split('=')[1].strip())
        elif ':' in item and 'ms' not in item:
            current_operation = item.replace(':', '').strip()
        elif 'ms' in item:
            data_structure, time = item.split(':')
            data_structure = data_structure.strip()
            if 'BinomialHeap' in data_structure:
                binomial_data['Size'].append(current_size)
                binomial_data['Operation'].append(current_operation)
                binomial_data['Data Structure'].append('BinomialHeap')
                binomial_data['Time (ms)'].append(float(time.strip().replace(' ms', '')))
            elif 'FibonacciHeap' in data_structure:
                fibonacci_data['Size'].append(current_size)
                fibonacci_data['Operation'].append(current_operation)
                fibonacci_data['Data Structure'].append('FibonacciHeap')
                fibonacci_data['Time (ms)'].append(float(time.strip().replace(' ms', '')))

    binomial_df = pd.DataFrame(binomial_data)
    fibonacci_df = pd.DataFrame(fibonacci_data)

    return binomial_df, fibonacci_df


if __name__ == "__main__":

    file_path = 'data.xlsx'
    data = pd.read_excel(file_path)

    raw_data = data.values.flatten()

    parsed_df1,parsed_df2 = parse_raw_data(raw_data)
    pd.DataFrame(data=parsed_df1)
    pd.DataFrame(data=parsed_df2)
    parsed_df1.to_excel("Binomial.xlsx")
    parsed_df2.to_excel("Fibonacci.xlsx")

    file_path = 'Fibonacci.xlsx'
    df = pd.read_excel(file_path)

    df_pivot = df.pivot(index='Size', columns='Operation', values='Time (ms)')

    df_pivot.reset_index(inplace=True)
    df_pivot.columns.name = None

    output_file_path = 'Fibonacci_fixed.xlsx'
    df_pivot.to_excel(output_file_path, index=False)

    print(f"Преобразованная таблица сохранена в {output_file_path}")
    file_path2 = 'Binomial.xlsx'
    df2 = pd.read_excel(file_path2)

    df_pivot2 = df2.pivot(index='Size', columns='Operation', values='Time (ms)')

    df_pivot2.reset_index(inplace=True)
    df_pivot2.columns.name = None

    output_file_path2 = 'Binomial_fixed.xlsx'
    df_pivot2.to_excel(output_file_path2, index=False)

    print(f"Преобразованная таблица сохранена в {output_file_path2}")

# a = []
# b = []
# for i in range (1000,50001,1000):
#     b.append(i)
# for j in range(len(a)):
#     a[j] = a[j] / b[j]
# for k in range(len(a)):
#     print(round(a[k],6))  