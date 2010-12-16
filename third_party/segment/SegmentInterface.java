import static net.sourceforge.segment.util.Util.getFileInputStream;
import static net.sourceforge.segment.util.Util.getReader;
import static net.sourceforge.segment.util.Util.getResourceStream;

import java.io.IOException;
import java.io.Reader;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import net.sourceforge.segment.SegmentTestSuite;
import net.sourceforge.segment.TextIterator;
import net.sourceforge.segment.Version;
import net.sourceforge.segment.srx.SrxDocument;
import net.sourceforge.segment.srx.SrxParser;
import net.sourceforge.segment.srx.SrxTransformer;
import net.sourceforge.segment.srx.SrxTextIterator;
import net.sourceforge.segment.srx.io.Srx1Transformer;
import net.sourceforge.segment.srx.io.SrxAnyParser;
import net.sourceforge.segment.srx.io.SrxAnyTransformer;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


/**
 * Batch interface to Segment dedicated for Pantera Tagger.
 * Based on original console UI from Segment library.
 * 
 * @author accek
 */
public class SegmentInterface {
	
	private static final Log log = LogFactory.getLog(SegmentInterface.class);
	
	private enum Algorithm {
		accurate, fast, ultimate;
	}

	public static final String DEFAULT_SRX = "net/sourceforge/segment/res/xml/default.srx";

	public static final String EOLN = System.getProperty("line.separator");
	
	public static final String DEFAULT_BEGIN_SEGMENT = "";
	public static final String DEFAULT_END_SEGMENT = EOLN;
	
	/* These constants apply to text / SRX generation */
	public static final int WORD_LENGTH = 2;
	public static final int SENTENCE_LENGTH = 5;
	
	public static void main(String[] args) {
		try {
			SegmentInterface main = new SegmentInterface();
			main.run(args);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	
	public SegmentInterface() {
	}

	private void run(String[] args) throws Exception {
		Options options = createOptions();
		HelpFormatter helpFormatter = new HelpFormatter();
		CommandLineParser parser = new PosixParser();
		CommandLine commandLine = null;

		try {
			commandLine = parser.parse(options, args);

			if (commandLine.hasOption('h')) {
				printHelp(options, helpFormatter);
			} else {
				segment(commandLine);
			}

		} catch (ParseException e) {
			printUsage(helpFormatter);
		} catch (IllegalArgumentException e) {
			System.err.println(e);
		}
		
	}
	
	private Options createOptions() {
		Options options = new Options();
		options.addOption("s", "srx", true, "SRX file.");
		options.addOption("l", "language", true, "Language code.");
		options.addOption("m", "map", true, "Map rule name in SRX 1.0.");
		options.addOption(null, "lookbehind", true, "Maximum length of a regular expression construct that occurs in lookbehind. Default: " + SrxTextIterator.DEFAULT_MAX_LOOKBEHIND_CONSTRUCT_LENGTH + ".");
		options.addOption(null, "margin", true, "If rule is matched but its position is in the margin (position > bufferLength - margin) then the matching is ignored. Default " + SrxTextIterator.DEFAULT_MARGIN + ".");
		options.addOption(null, "null-delimiter", false, "Use NULL character as sentence delimiter.");
		options.addOption("h", "help", false, "Print this help.");
		return options;
	}
	
	private void printUsage(HelpFormatter helpFormatter) {
		System.err.println("Unknown command. Use segment -h for help.");
	}
	
	private void printHelp(Options options, HelpFormatter helpFormatter) {
		String signature = "Segment";
		if (Version.getInstance().getVersion() != null) {
			signature += " " + Version.getInstance().getVersion();
		}
		if (Version.getInstance().getDate() != null) {
			signature += ", " + Version.getInstance().getDate();
		}
		signature += ".";
		System.err.println(signature);
		helpFormatter.printHelp("segment", options);
	}
	
	private void segment(CommandLine commandLine) throws IOException {
        SrxDocument document = createSrxDocument(commandLine);
        Scanner inputScanner = new Scanner(System.in);

        String languageCode = commandLine.getOptionValue('l');
        if (languageCode == null) {
            languageCode = "";
        }
        
        Map<String, Object> parameterMap =
            createTextIteratorParameterMap(commandLine);

        String delimiter = "|";
        if (commandLine.hasOption("null-delimiter"))
            delimiter = "\0";
        
        while (inputScanner.hasNextLine()) {
            TextIterator textIterator = new SrxTextIterator(document, languageCode,
                    inputScanner.nextLine(), parameterMap);
            while (textIterator.hasNext()) {
                String segment = textIterator.next();
                System.out.print(delimiter);
                System.out.print(segment);
            }
            System.out.println();
            System.out.flush();
        }
	}
	
	private SrxDocument createSrxDocument(CommandLine commandLine) throws IOException {
		SrxDocument document;

		long start = System.currentTimeMillis();

        String fileName = commandLine.getOptionValue('s');
        String mapRule = commandLine.getOptionValue('m');
        return readSrxDocument(fileName, mapRule);
	}

	private SrxDocument readSrxDocument(String fileName, String mapRule) 
			throws IOException {
		Reader srxReader;

		if (fileName != null) {
			srxReader = getReader(getFileInputStream(fileName));
		} else {
			srxReader = getReader(getResourceStream(DEFAULT_SRX));
		}

		Map<String, Object> parameterMap = new HashMap<String, Object>();

		if (mapRule != null) {
			parameterMap.put(Srx1Transformer.MAP_RULE_NAME, mapRule);
		}

		// If there are transformation parameters then separate transformation
		// is needed.
		if (parameterMap.size() > 0) {
			SrxTransformer transformer = new SrxAnyTransformer();
			srxReader = transformer.transform(srxReader, parameterMap);
		}

		SrxParser srxParser = new SrxAnyParser();
		SrxDocument document = srxParser.parse(srxReader);
		srxReader.close();

		return document;
	}

    private Map<String, Object> createTextIteratorParameterMap(CommandLine commandLine) {
		Map<String, Object> parameterMap = new HashMap<String, Object>();
		if (commandLine.hasOption("lookbehind")) {
			parameterMap.put(
					SrxTextIterator.MAX_LOOKBEHIND_CONSTRUCT_LENGTH_PARAMETER, 
					Integer.parseInt(commandLine.getOptionValue("lookbehind")));
		}
		if (commandLine.hasOption("margin")) {
			parameterMap.put(
					SrxTextIterator.MARGIN_PARAMETER, 
					Integer.parseInt(commandLine.getOptionValue("margin")));
		}

        return parameterMap;
    }
}
