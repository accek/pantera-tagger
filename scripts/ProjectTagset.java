/*
 * (C) Danuta Karwanska, 2009
 * 
 * The code was obtained from the authors of
 *
 *   Danuta Karwańska and Adam Przepiórkowski. (2009).
 *   "On the Evaluation of Two Polish Taggers."
 *   To appear in the proceedings of PALC 2009.
 *
 * Redistributed with permission under the terms of the GNU GPL v3.
 */

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

/*
 * Zalozenie, ze w XML ma taka postac, ze w jednej 
 * linijce znajduje sie 1 lub 0 <orth>...</orth>
 */

public class ProjectTagset {
        
    public static void main(String[] args) throws 
                IOException {
            
        System.out.println("POCZATEK");
            
        FileInputStream fis = null;
        InputStreamReader isr = null;
        BufferedReader br = null;
            
        FileOutputStream fos = null;
        OutputStreamWriter osw = null;
        BufferedWriter bw = null;
                    
        if (args.length < 2) {
            System.err.println("Too few arguments");
            System.exit(0);
        }

        try {
            
            for (int i = 0; i < 10; i++) {
                    
                if (i == 0 || i == 4 || i == 6) {
                    continue;
                }
                    
                fis = new FileInputStream(args[0]);
                isr = new InputStreamReader(fis, "UTF-8");
                br = new BufferedReader(isr);
                    
                fos = new FileOutputStream(args[1]);
                osw = new OutputStreamWriter(fos, "UTF-8");
                bw = new BufferedWriter(osw);
                  
                String s = br.readLine();
                  
                while (s != null) {
                    
                    projectAndPrint(s, bw);
                    s = br.readLine();
                }
                  
                fis.close();
                isr.close();
                br.close();
    
                bw.close();
                fos.close();
                osw.close();
            }
            System.out.println("KONIEC");
                
        } catch (IOException e) {
            if (fis != null) {
                fis.close();
            }
            if (isr != null) {
                isr.close();
            }
            if (br != null) {
                br.close();
            }
            if (bw != null) {
                bw.close();
            }
            if (fos != null) {
                fos.close();
            }
            if (osw != null) {
                osw.close();
            }
            throw(e);
        }   
            
    }
        
    private static void projectAndPrint(String s, BufferedWriter bw) 
        throws IOException {
            
        if (!s.contains("<ctag>")) {
            bw.write(s);
            bw.write("\n");
        } else {
    
            String[] tabs1 = s.split("<ctag>");
            String[] tabs2 = tabs1[1].split("</ctag>");

            project(tabs2[0], tabs1[0], tabs2[1], bw);
            
        }
    }
        
    private static void project(
            String s, String begin, 
            String end, BufferedWriter bw) throws IOException {
            
        if (s.contains("num")) {
            projectAndPrintNum(s, begin, end, bw);
        } else if (s.contains("ppron3")) {
            projectAndPrintPpron3(s, begin, end, bw);
        } else {
            printLineWithCtag(s, begin, end, bw);
        }
    }

    private static void printLineWithCtag(String s, String begin, String end,
            BufferedWriter bw) throws IOException {
        bw.write(begin);
        bw.write("<ctag>");
        bw.write(s);
        bw.write("</ctag>");
        bw.write(end);
        bw.write("\n");
    }
        
    private static void projectAndPrintNum(String s, String begin, 
            String end, BufferedWriter bw) throws IOException {
            
        String[] tabs = s.split(":");
        String s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3];

        printLineWithCtag(s1, begin, end, bw);
            
    }
        
    private static void projectAndPrintPpron3(String s, String begin, 
            String end, BufferedWriter bw) throws IOException {
            
        if ((s.contains("akc") || s.contains("nakc"))
                && (s.contains("praep") || s.contains("npraep"))) {
            printLineWithCtag(s, begin, end, bw);
        } else if (s.contains("praep") || s.contains("npraep")) {
            String s1 = null;
            // System.out.println(s);
            String[] tabs = s.split(":");

            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":akc:" + tabs[5]; 
            printLineWithCtag(s1, begin, end, bw);

            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":nakc:" + tabs[5]; 
            printLineWithCtag(s1, begin, end, bw);
        } else {
            String s1 = null;
            // System.out.println(s);
            String[] tabs = s.split(":");

            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":akc:praep"; 
            printLineWithCtag(s1, begin, end, bw);
                
            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":akc:npraep"; 
            printLineWithCtag(s1, begin, end, bw);

            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":nakc:praep"; 
            printLineWithCtag(s1, begin, end, bw);

            s1 = tabs[0] + ":" + tabs[1] + ":" + tabs[2] + ":" + tabs[3] + ":"
                    + tabs[4] + ":nakc:npraep"; 
            printLineWithCtag(s1, begin, end, bw);
        }
    }
}
