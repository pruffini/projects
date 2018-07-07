
import com.amazonservices.mws.orders._2013_09_01.MarketplaceWebServiceOrders;
import com.amazonservices.mws.orders._2013_09_01.MarketplaceWebServiceOrdersAsyncClient;
import com.amazonservices.mws.orders._2013_09_01.MarketplaceWebServiceOrdersClient;
import com.amazonservices.mws.orders._2013_09_01.MarketplaceWebServiceOrdersConfig;
import com.amazonservices.mws.orders._2013_09_01.MarketplaceWebServiceOrdersException;
import com.amazonservices.mws.client.*;
import com.amazonservices.mws.orders._2013_09_01.*;
import com.amazonservices.mws.orders._2013_09_01.model.*;
import com.amazonservices.mws.orders._2013_09_01.samples.ListOrderItemsSample;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Enumeration;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.Properties;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;

import java.io.File;

/**
 *
 * @author Philip Ruffini
 */
public class GetZipCodeAmazon {
 
    //here we get the data from config txt file, don't change here
    private static String Access_Key = null;
    private static String Secrete_Key = null;
    private static String Seller_ID = null;
    private static String MWS_AuthToken = null;
    private static String CreateAfterDate = null;
    private static String CreateBeforeDate = null;
    private static String MarketPlaceID = null;
    
    
    //open the config txt file and get values 
     static {
        try {
            Properties config = new Properties();
            try (InputStream in = new FileInputStream("config.txt")) {
                config.load(in);
            }

            Access_Key = config.getProperty("Access_Key");
            Secrete_Key = config.getProperty("Secrete_Key");
            Seller_ID = config.getProperty("Seller_ID");
            MWS_AuthToken = config.getProperty("MWS_AuthToken");
            CreateAfterDate = config.getProperty("CreateAfterDate");
            CreateBeforeDate = config.getProperty("CreateBeforeDate");
            MarketPlaceID = config.getProperty("MarketPlaceID");
            
            
        } catch (Exception e) {
        }
     }
        
    /**
     * args the command line arguments
     */
    public static void main(String[] args)  {
        System.out.println("Starting..."); 

         // Get a client connection.
        // Make sure you've set the variables in MarketplaceWebServiceOrdersSampleConfig.
        MarketplaceWebServiceOrdersClient client = getClient(Access_Key,Secrete_Key);

        // Create a request.
        ListOrdersRequest request = new ListOrdersRequest();
        String sellerId = Seller_ID;
        request.setSellerId(sellerId);
        String mwsAuthToken = MWS_AuthToken;
        request.setMWSAuthToken(mwsAuthToken);
        
        
        //read Required request parameters here - https://docs.developer.amazonservices.com/en_UK/dev_guide/DG_RequiredRequestParameters.html
        
        //set Create After Date
        XMLGregorianCalendar CreatedAfterxmlGregCal = null;
        try 
        {
            DateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
            Date date = format.parse(CreateAfterDate+" 00:00:00");
            GregorianCalendar cal = new GregorianCalendar();
            cal.setTime(date);
            CreatedAfterxmlGregCal =  DatatypeFactory.newInstance().newXMLGregorianCalendar(cal);
            System.out.println(CreatedAfterxmlGregCal);
        } 
        catch (ParseException | DatatypeConfigurationException e) 
        {          
        }
        
        //set Create Before Date
        XMLGregorianCalendar CreatedBeforexmlGregCal=null;
        try 
        {
            DateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
            Date date2 = format.parse(CreateBeforeDate+" 00:00:00");
            GregorianCalendar cal = new GregorianCalendar();
            cal.setTime(date2);
            CreatedBeforexmlGregCal=  DatatypeFactory.newInstance().newXMLGregorianCalendar(cal);
            System.out.println(CreatedBeforexmlGregCal);
        } 
        catch (ParseException | DatatypeConfigurationException e) 
        {           
        }

        //assign create after date
        request.setCreatedAfter(CreatedAfterxmlGregCal);        
        //assign create before date 
        request.setCreatedBefore(CreatedBeforexmlGregCal);
        
        // assign market place
        List<String> marketplaceId = new ArrayList<>();
        marketplaceId.add(MarketPlaceID);
        request.setMarketplaceId(marketplaceId);
       
        //Save to Txt File ZipCodes.txt
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(Calendar.getInstance().getTime());
        String fileName = "AmazonData.txt";
        Vector<String> idAndZipVector;
        idAndZipVector = new Vector<String>();
        int count =1;
        boolean bool = true;
        
        System.out.println("Downloading Orders...!!!");
        
        // Make the call.  and get response
        ListOrdersResponse response =invokeListOrders(client, request);   
        String responseXml = response.toXML();
        
        //the response will save as txt file.. if you dont need you can comment it
        try (PrintWriter writer = new PrintWriter("Amazon_Response.txt")) {
            writer.println(responseXml);
        } catch (FileNotFoundException ex) {
            Logger.getLogger(GetZipCodeAmazon.class.getName()).log(Level.SEVERE, null, ex);
        }
                    
        //here, response will split by orders
        String[] orders = responseXml.split("<AmazonOrderId>");
        int x=0;
        for (String order : orders) 
        {                        
            //skip the first run, because its containing no data
            if (order.equals(orders[0])) continue; 
            String amazonId = order.substring(0,19);    
            try 
            {
                String[] PDate = order.split("<PurchaseDate>");
                String PurchaseDate = PDate[1].substring(0,10);                                 
        
                String[] zip = order.split("PostalCode");            
                String zipCode= zip[1].replace(">", "").replace("</","");   //>85710-3125</
                
                String[] address = order.split("<ShippingAddress>");
                String[] Address2 = address[1].split("</ShippingAddress>");
                String FullAddress = Address2[0].replace("<Name>", " ").replace("</Name>",", ").replace("<AddressLine1>", " ").replace("</AddressLine1>", " ").replace("<AddressLine2>", " ").replace("</AddressLine2>", " ").replace("<City>", " ").replace("</City>", " ").replace("<StateOrRegion>", " ").replace("</StateOrRegion>", " ").replace("<PostalCode>", " ").replace("</PostalCode>", " ").replace("<CountryCode>", " ").replace("</CountryCode>", "");
                //System.out.println(FullAddress);
                
                String productName = getProductName(amazonId,client);    //output is = ASIN : Title        
                Thread.sleep(1000);
                
                //orderId : date : asin : tilte : address : zipcode
                idAndZipVector.addElement(amazonId + " : "+ PurchaseDate + " : " + productName+" : "+FullAddress);  // adding amazon id and zipcode to vector
                String idAndZip =amazonId + "\t : "+PurchaseDate + "\t : "+productName+ "\t : "+FullAddress+"\n";
                writeFile(idAndZip,fileName);  // adding to txt file
                System.out.println(idAndZip);
                if(x>50)
                {
                    try 
                    {
                        System.out.println("Waiting 30 Seconds to reset API calls...!");
                        Thread.sleep(30000);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(GetZipCodeAmazon.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    x=0;
                }
                x++;
            } 
            catch (Exception e) 
            {
                //JOptionPane.showMessageDialog(null, e.toString(), "InfoBox", JOptionPane.INFORMATION_MESSAGE);
            }       
        }
        
        

        String[] nextToken = null , nextToken2 = null;
                    try 
                    {
                        nextToken = responseXml.split("<NextToken>");
                        nextToken2 = nextToken[1].split("</NextToken>"); //nextToken2[0].toString()
                    } catch (Exception e) 
                    {          
                        bool=false;
                        //JOptionPane.showMessageDialog(null, e.toString(), "next token split", JOptionPane.INFORMATION_MESSAGE);
                    }
        while (bool) 
        {            
            try 
                {               
                if(nextToken.length>1)
                {                                      
                    // Create a request.
                    ListOrdersByNextTokenRequest request2 = new ListOrdersByNextTokenRequest();      
                    request2.setSellerId(sellerId);       
                    request2.setMWSAuthToken(mwsAuthToken);
                    request2.setNextToken(nextToken2[0]);

                    System.out.println("Downloading Orders...!!!");
                    // Make the call.
                    ListOrdersByNextTokenResponse responseNext=invokeListOrdersByNextToken(client, request2);
                    String responseNextXml = responseNext.toXML();
                    
                    try 
                    {
                        nextToken = responseNextXml.split("<NextToken>");
                        nextToken2 = nextToken[1].split("</NextToken>"); //nextToken2[0].toString()
                    } catch (Exception e) 
                    {          
                        bool=false;
                        //JOptionPane.showMessageDialog(null, e.toString(), "no next token split", JOptionPane.INFORMATION_MESSAGE);
                    }
                    
                    //here, response will split by orders
                    String[] ordersNext = responseNextXml.split("<AmazonOrderId>");        
                    for (String order : ordersNext) 
                    {                        
                        //skip the first run, because its containing no data
                        if (order.equals(ordersNext[0])) continue; 
                        String amazonId = order.substring(0,19);              
                        try 
                        {                         
                            String[] PDate = order.split("<PurchaseDate>");
                            String PurchaseDate = PDate[1].substring(0,10);                                 

                            String[] zip = order.split("PostalCode");            
                            String zipCode= zip[1].replace(">", "").replace("</","");   //>85710-3125</

                            String[] address = order.split("<ShippingAddress>");
                            String[] Address2 = address[1].split("</ShippingAddress>");
                            String FullAddress = Address2[0].replace("<Name>", " ").replace("</Name>",", ").replace("<AddressLine1>", " ").replace("</AddressLine1>", " ").replace("<AddressLine2>", " ").replace("</AddressLine2>", " ").replace("<City>", " ").replace("</City>", " ").replace("<StateOrRegion>", " ").replace("</StateOrRegion>", " ").replace("<PostalCode>", " ").replace("</PostalCode>", " ").replace("<CountryCode>", " ").replace("</CountryCode>", "");
                            //System.out.println(FullAddress);

                            String productName = getProductName(amazonId,client);    //output is = ASIN : Title        
                            Thread.sleep(1000);

                            //orderId : date : asin : tilte : address : zipcode
                            idAndZipVector.addElement(amazonId + " : "+ PurchaseDate + " : " + productName+" : "+FullAddress);  // adding amazon id and zipcode to vector
                            String idAndZip =amazonId + "\t : "+PurchaseDate + "\t : "+productName+ "\t : "+FullAddress+"\n";
                            writeFile(idAndZip,fileName);  // adding to txt file
                            System.out.println(x+" - " +idAndZip);
                            if(x>50)
                            {
                                try 
                                {
                                    System.out.println("Waiting 30 Seconds to reset API calls...!");
                                    Thread.sleep(30000);
                                } catch (InterruptedException ex) {
                                    Logger.getLogger(GetZipCodeAmazon.class.getName()).log(Level.SEVERE, null, ex);
                                }
                                x=0;
                            }
                            x++;
                        } 
                        catch (Exception e) 
                        {
                            //JOptionPane.showMessageDialog(null, e.toString(), "InfoBox", JOptionPane.INFORMATION_MESSAGE);
                        }       
                    }
                }
                else
                {
                    bool = false;
                    //JOptionPane.showMessageDialog(null, "else bool throw", "else throw error", JOptionPane.INFORMATION_MESSAGE);
                }
            }
            catch (Exception e) 
            {                  
                bool=false;
                //JOptionPane.showMessageDialog(null, e.toString(), "NextToken throws error", JOptionPane.INFORMATION_MESSAGE);
            }
            
        }
        
        
          
        
        //traversing elements using Enumeration  
        Enumeration e=idAndZipVector.elements();  
        while(e.hasMoreElements())
        {  
         System.out.println(e.nextElement());
         count++;
        }
        
        System.out.println("Program End...");

    }
    
    public static void writeFile(String line, String fileName)
    {  
        try ( FileWriter fw = new FileWriter(fileName,true))   //the true will append the new data
        {
            fw.write(line);    //appends the string to the file
        } catch (IOException ex) {
            Logger.getLogger(GetZipCodeAmazon.class.getName()).log(Level.SEVERE, null, ex);
        }           
    }
    
    public static ListOrdersResponse invokeListOrders(
            MarketplaceWebServiceOrders client, 
            ListOrdersRequest request) {
        try {
            // Call the service.
            ListOrdersResponse response = client.listOrders(request);                
            ResponseHeaderMetadata rhmd = response.getResponseHeaderMetadata();
            // We recommend logging every the request id and timestamp of every call.
            System.out.println("Response: OK");
            System.out.println("RequestId: "+rhmd.getRequestId());
            System.out.println("Timestamp: "+rhmd.getTimestamp());
            System.out.println("Loading...");
            //String responseXml = response.toXML();
            //System.out.println(responseXml);
            //JOptionPane.showMessageDialog(null, responseXml.toString(), "InfoBox", JOptionPane.INFORMATION_MESSAGE);
            return response;
        } catch (MarketplaceWebServiceOrdersException ex) {
            //JOptionPane.showMessageDialog(null, ex.toString(), "InfoBox", JOptionPane.INFORMATION_MESSAGE);
            
            // Exception properties are important for diagnostics.
            System.out.println("Service Exception:");
            System.out.println("Message: "+ex.getMessage());
            System.out.println("StatusCode: "+ex.getStatusCode());
            System.out.println("ErrorCode: "+ex.getErrorCode());
            System.out.println("ErrorType: "+ex.getErrorType());
            throw ex;
        }
    }
    
    
    
    /** Developer AWS access key. */
    //private static final String accessKey = "AKIAJ3HLT6DGWTFFJQ5Q";

    /** Developer AWS secret key. */
    //private static final String secretKey ="qzyyRTtXTtPcPzUnNIxUTeI8w4ZiwuXD75YTqbtV";

    /** The client application name. */
    private static final String appName = "getOrders";

    /** The client application version. */
    private static final String appVersion = "2";

    /**
     * The endpoint for region service and version.
     * ex: serviceURL = MWSEndpoint.NA_PROD.toString();
     */
    private static final String serviceURL = "https://mws.amazonservices.com/";

    /** The client, lazy initialized. Async client is also a sync client. */
    private static MarketplaceWebServiceOrdersAsyncClient clientmws = null;

    /**
     * Get a client connection ready to use.
     *
     * @param accessKey
     * @param secretKey
     * @return A ready to use client connection.
     */
    public static MarketplaceWebServiceOrdersClient getClient(String accessKey, String secretKey) {            
        return getAsyncClient(accessKey, secretKey);
    }

    /**
     * Get an async client connection ready to use.
     *
     *
     * @return A ready to use client connection.
     */
    public static synchronized MarketplaceWebServiceOrdersAsyncClient getAsyncClient(String accessKey, String secretKey) {
        if (clientmws==null) {
            MarketplaceWebServiceOrdersConfig config = new MarketplaceWebServiceOrdersConfig();
            config.setServiceURL(serviceURL);
            // Set other client connection configurations here.
            clientmws = new MarketplaceWebServiceOrdersAsyncClient(accessKey, secretKey, 
                    appName, appVersion, config, null);
        }
        return clientmws;
    }

    
     public static ListOrdersByNextTokenResponse invokeListOrdersByNextToken(
            MarketplaceWebServiceOrders client, 
            ListOrdersByNextTokenRequest request) {
        try {
            // Call the service.
            ListOrdersByNextTokenResponse response = client.listOrdersByNextToken(request);
            ResponseHeaderMetadata rhmd = response.getResponseHeaderMetadata();
            // We recommend logging every the request id and timestamp of every call.
            System.out.println("Response:OK");
            System.out.println("RequestId: "+rhmd.getRequestId());
            System.out.println("Timestamp: "+rhmd.getTimestamp());
            System.out.println("Loading...");
            //String responseXml = response.toXML();
            //System.out.println(responseXml);
            return response;
        } catch (MarketplaceWebServiceOrdersException ex) {
            // Exception properties are important for diagnostics.
            System.out.println("Service Exception:");
            ResponseHeaderMetadata rhmd = ex.getResponseHeaderMetadata();
            if(rhmd != null) {
                System.out.println("RequestId: "+rhmd.getRequestId());
                System.out.println("Timestamp: "+rhmd.getTimestamp());
            }
            System.out.println("Message: "+ex.getMessage());
            System.out.println("StatusCode: "+ex.getStatusCode());
            System.out.println("ErrorCode: "+ex.getErrorCode());
            System.out.println("ErrorType: "+ex.getErrorType());
            throw ex;
        }
    }

    public static String getProductName(String amazonId,MarketplaceWebServiceOrdersClient client) 
    {
        try 
        {
            // Create a request.
            ListOrderItemsRequest request = new ListOrderItemsRequest();
            request.setSellerId(Seller_ID);
            request.setMWSAuthToken(MWS_AuthToken);
            request.setAmazonOrderId(amazonId);

            // Make the call.
            ListOrderItemsResponse response = invokeListOrderItems(client, request);
            String responseProduct = response.toXML();
            //System.out.println(responseProduct);
            String[] ProInfo = responseProduct.split("<Title>");
            String[] ProInfo2 = ProInfo[1].split("</Title>");
            
            String[] Asin = responseProduct.split("<ASIN>");
            String[] AsinCode = Asin[1].split("</ASIN>");

            return AsinCode[0]+" : "+ProInfo2[0];    
        } catch (Exception e) {
            //JOptionPane.showMessageDialog(null, e.toString(), "pr error", JOptionPane.INFORMATION_MESSAGE);
        }
        return null;
    }
    
    public static ListOrderItemsResponse invokeListOrderItems(
            MarketplaceWebServiceOrders client, 
            ListOrderItemsRequest request) {
        try {
            // Call the service.
            ListOrderItemsResponse response = client.listOrderItems(request);        
            return response;
        } catch (MarketplaceWebServiceOrdersException ex) {
            // Exception properties are important for diagnostics.
            System.out.println("Service Exception: Getting Product Information");      
            System.out.println("Message: "+ex.getMessage());
            System.out.println("StatusCode: "+ex.getStatusCode());
            System.out.println("ErrorCode: "+ex.getErrorCode());
            System.out.println("ErrorType: "+ex.getErrorType());
            throw ex;
        }
    }
    
    

    
    
}


