import java.util.LinkedList;
import java.util.List;

public class CongruenciaLineal {
    
    public static List<Integer> generateRandomNumbers (int X0, int a, int c, int m, int numberOfNumbers) {
        if (numberOfNumbers < 0) throw new IllegalArgumentException(); 
        if (numberOfNumbers == 0) return new LinkedList<>();  
        
        List<Integer> answer = new LinkedList<>();
        int XiAnterior = X0;
        for (int i = 1; i <= numberOfNumbers; i++) {
            int Xi = (a * XiAnterior + c) % m;
            answer.add(Xi);
            XiAnterior = Xi;
        }

        return answer;
    }

}