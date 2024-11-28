package themepark;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws Exception {
		Scanner scanner = new Scanner(new File("themepark/test.in"));
		BufferedWriter out = new BufferedWriter(new OutputStreamWriter(
				new FileOutputStream("themepark/result.txt")));
		int cases = scanner.nextInt();

		LinkedList<Integer> list;
		int R;
		int k;
		int N;
		long result;

		for (int i = 0; i < cases; i++) {
			System.out.println("test " + (i + 1));
			list = new LinkedList<Integer>();
			result = 0;
			R = scanner.nextInt();
			k = scanner.nextInt();
			N = scanner.nextInt();
			for (int j = 0; j < N; j++) {
				list.addLast(scanner.nextInt());
			}
			current = 0;
			groupCount = new long[list.size()];
			Arrays.fill(groupCount, -1);
			peopleCount = new long[list.size()];
			Arrays.fill(peopleCount, -1);
			for (int j = 0; j < R; j++) {
				result += nextTurn(list, k);
			}

			out.write("Case #" + (i + 1) + ": " + result);
			out.newLine();
		}
		out.flush();
	}

	static int current;
	static long[] groupCount;
	static long[] peopleCount;

	static long nextTurn(LinkedList<Integer> list, int k) {
		long result = 0;
		if (groupCount[current] != -1) {
			result = peopleCount[current];
			current += groupCount[current];
			current %= list.size();
			return result;
		}

		groupCount[current] = 0;
		int len = list.size();
		while (k - list.getFirst() >= 0 && len > 0) {
			k -= list.getFirst();
			result += list.getFirst();
			list.add(list.removeFirst());
			len--;
			groupCount[current]++;
		}
		peopleCount[current] = result;
		current += groupCount[current];
		current %= list.size();

		return result;
	}

}
