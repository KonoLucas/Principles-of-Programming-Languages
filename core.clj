(ns a3.core
  (:gen-class)
  (:require [clojure.string :as str]))

;; Parses the event text and constructs a map of event details.
(defn parse-event [event-text]
  (let [lines (str/split-lines event-text)
        date (second (str/split (nth lines 0) #": "))
        location (second (str/split (nth lines 1) #": "))
        type (second (str/split (nth lines 2) #": "))
        significance (second (str/split (nth lines 3) #": "))]
    {:date date :location location :type type :significance significance}))

;; Reads all event entries from a file, parses each event, and returns a vector of events.
(defn read-events [filename]
  (let [content (slurp filename)
        events (str/split content #"(?:\r\n\r\n|\n\n)")]
    ;;(println "Parsed events:" events) 
    (vec (map parse-event events))))

;; Displays all eclipse events in the list with their details.
(defn display-eclipse-events [eclipse-list]
  (println (str "Total number of eclipse events: " (count eclipse-list)))
  (doseq [event eclipse-list]
    (println "Date:" (:date event) ", Location:" (:location event)
             ", Type:" (:type event) ", Significance:" (:significance event))))

;; Prompts the user to enter details for a new eclipse event and returns a map of these details.
(defn prompt-for-event-details []
  {:date (do (println "Enter Date:") (read-line))
   :location (do (println "Enter Location:") (read-line))
   :type (do (println "Enter Type:") (read-line))
   :significance (do (println "Enter Significance:") (read-line))})

;; Adds a new eclipse event to the list and returns the updated list.
(defn add-eclipse-event [eclipse_list]
  (let [new-event (prompt-for-event-details)
        updated-list (cons new-event eclipse_list)]
    (println "Successfully added new eclipse event!")
    updated-list))

;; Displays all eclipse events with an index for each, aiding in selection for modifications.
(defn display-eclipse-events-with-index [eclipse_list]
  (println "Total number of eclipse events:" (count eclipse_list))
  (doseq [[idx event] (map-indexed vector eclipse_list)]
    (println (str idx ". Date:" (:date event) ", Location:" (:location event)
                  ", Type:" (:type event) ", Significance:" (:significance event)))))

;; Modifies an existing eclipse event based on the user's selection of the event index.
(defn modify-eclipse-event [eclipse_list]
  (println "Please select the number of the event you wish to modify:")
  (let [input (read-line)
        index (try (Integer/parseInt input) (catch NumberFormatException e (println "Invalid input: " (.getMessage e) nil)))]
    (if (and index (>= index 0) (< index (count eclipse_list)))
      (let [new-details (prompt-for-event-details)
            updated-events (assoc eclipse_list index new-details)]
        (println "The eclipse event has been successfully updated.")
        updated-events)
      (println "Invalid event number selected."))))

;; Searches for eclipse events by date and displays matching events.
(defn search-for-eclipse-events_bydate [eclipse_list]
  (println "Please enter a Date to search for :")
  (let [search-term (str/lower-case (read-line))
        matching-events (filter #(str/includes? (str/lower-case (:date %)) search-term) eclipse_list)]
    (if (empty? matching-events)
      (println "No events found for the given date.")
      (doseq [event matching-events]
        (println "Date:" (:date event) ", Location:" (:location event) ", Type:" (:type event) ", Significance:" (:significance event))))))

(defn search-for-eclipse-events_bylocation [eclipse_list]
  (println "Please enter a location to search for :")
  (let [search-term (str/lower-case (read-line))
        matching-events (filter #(str/includes? (str/lower-case (:location %)) search-term) eclipse_list)]
    (if (empty? matching-events)
      (println "No events found for the given location.")
      (doseq [event matching-events]
        (println "Date:" (:date event) ", Location:" (:location event) ", Type:" (:type event) ", Significance:" (:significance event))))))



(defn menu []
  (println "1. View Eclipse Events")
  (println "2. Add New Eclipse Event")
  (println "3. Modify Eclipse Event")
  (println "4. Search for Eclipse Events")
  (println "5. Exit")
  (print "Enter your choice (1-5): "))

(defn get-user-input [eclipse_list]
  (loop []
    (menu)
    (println "Please enter a number (1-5):")
    (let [input (read-line)
          parsed-input (try (Integer/parseInt input)
                            (catch NumberFormatException e
                              (println "Invalid input: " (.getMessage e))
                              nil))]
      (when (nil? parsed-input)
        (println "Invalid input. Please enter a valid number."))
      (case parsed-input
        1 (do (display-eclipse-events eclipse_list) (get-user-input eclipse_list))
        2 ((get-user-input (add-eclipse-event eclipse_list)))
        3 (do (display-eclipse-events-with-index eclipse_list)
              (get-user-input (modify-eclipse-event eclipse_list)))
        4 (do (println "Enter search by 'Date' or 'Location':")
              (let [input (read-line)]
                (case (str/lower-case input)
                  "date" (do (search-for-eclipse-events_bydate eclipse_list)
                             (get-user-input eclipse_list))
                  "location" (do (search-for-eclipse-events_bylocation eclipse_list)
                                 (get-user-input eclipse_list))
                  (println "Invalid input. Please enter 'Date' or 'Location'.")
                  (get-user-input eclipse_list))))
        5 (do (println "Exiting program.") ((System/exit 0)) )
        :else (do (println "Invalid input. Please enter a number between 1 and 5.") (recur))))))


(defn -main []
  (println "=== Eclipse History Encyclopedia ===")
  (let [filename "eclipse_events.txt"
        eclipse_list (read-events filename)]
    (get-user-input eclipse_list)))