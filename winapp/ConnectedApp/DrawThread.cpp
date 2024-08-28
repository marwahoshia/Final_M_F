#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "DrawThread.h"
#include "GuiMain.h"
#include "../../shared/ImGuiSrc/imgui.h"

// Add a variable to track the selected job index
static int selected_job_index = -1; // Initialize the selected job index to -1, indicating no job is currently selected.
static int current_interface_state = 0; // 0 for intro, 1 for main interface

void DrawAppWindow(void* common_ptr) {
    auto common = static_cast<CommonObjects*>(common_ptr);

    // Introductory Interface
    if (current_interface_state == 0) {//first interface
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
        ImGui::Begin("Introduction", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

        // Center the window
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 window_pos = ImVec2((io.DisplaySize.x - 700) / 2, (io.DisplaySize.y - 500) / 2);  // Adjusted the window size
        ImGui::SetWindowPos(window_pos);

        // Draw the rectangle with the text
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.75f, 0.75f, 1.0f)); // Softer pink background
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.8f)); // Darker border
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f); // Rounded corners
        ImGui::BeginChild("TextChild", ImVec2(650, 250), true, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);

        // Center text within the rectangle
        ImVec2 text_size = ImGui::CalcTextSize("Welcome to Job Researcher!\nDiscover your next career opportunity with ease.\nJob Researcher allows you to search, view, and manage\njob listings tailored to your needs. Save your favorite\njobs and explore detailed descriptions to find the perfect\nmatch. Start your job search journey today !!");
        ImGui::SetCursorPosX((650 - text_size.x) / 2);  // Adjusted width to match the rectangle size
        ImGui::SetCursorPosY((250 - text_size.y) / 2);  // Adjusted height to match the rectangle size

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Set text color to black
        ImGui::TextWrapped("Welcome to Job Researcher!\nDiscover your next career opportunity with ease.\nJob Researcher allows you to search, view, and manage\njob listings tailored to your needs. Save your favorite\njobs and explore detailed descriptions to find the perfect\nmatch. Start your job search journey today !!");
        ImGui::PopStyleColor();  // Revert the text color to the previous color

        ImGui::EndChild();
        ImGui::PopStyleColor(2);  // Pop the border, text color, and child background color
        ImGui::PopStyleVar();  // Pop rounded corners

        // Add a "Next" button with a white background color and black text
        ImGui::SetCursorPosX((650 - 100) / 2); // Center the button relative to the new rectangle size
        ImGui::SetCursorPosY(window_pos.y + 270); // Position it below the text box
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 1.0f, 1.0f)); // Blue background color
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Black text color
        if (ImGui::Button("Next", ImVec2(100, 40))) {
            current_interface_state = 1; // Move to the main interface
        }
        ImGui::PopStyleColor(2);  // Pop the text color and button background color

        ImGui::End();
    }

    // Main Interface
    else if (current_interface_state == 1) {
        // Modify the background color here
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.96f, 0.96f, 0.86f, 1.0f));  // Beige background
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.94f, 0.90f, 0.75f, 1.0f));  // Slightly darker beige for frames
        ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.95f, 0.93f, 0.80f, 1.0f)); // Another shade of beige for table rows
        ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.96f, 0.96f, 0.86f, 1.0f)); // Matching the window background for alternative rows
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Black text color for visibility

        if (selected_job_index == -1) { // if the button show is not pressed
            // Set the title color to neon and text color to black for better contrast
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.1f, 1.0f, 0.8f, 1.0f)); // Neon-like cyan
            ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 1.0f, 0.8f, 1.0f)); // Neon-like cyan for inactive state
            ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.1f, 1.0f, 0.8f, 1.0f)); // Neon-like cyan for collapsed state
            ImGui::Begin("Job Researcher", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
            ImGui::PopStyleColor(3); // Pop the three title colors

            static char search_buffer[128] = "";
            static bool show_favorites_only = false;

            ImGui::InputText("Job Title", search_buffer, sizeof(search_buffer));

            // Change the "Search" button color to light blue
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 1.0f, 1.0f));
            if (ImGui::Button("Search")) {
                common->search_query = search_buffer;
                common->start_download = true;
                show_favorites_only = false;
            }
            ImGui::PopStyleColor(); // Revert to previous button color

            ImGui::SameLine();

            // Make the "Favorite Jobs" button green
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
            if (ImGui::Button("Favorite Jobs")) {
                show_favorites_only = true;
            }
            ImGui::PopStyleColor(); // Revert to previous button color

            if (common->data_ready) {
                // Set header colors to white
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // White header background
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));  // Slightly darker on hover

                if (ImGui::BeginTable("Jobs", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {// job title , Action,Fav,remove

                    // Set the header text color to black
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

                    ImGui::TableSetupColumn("Job Title", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Favorite", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow(); // Draw the table headers

                    ImGui::PopStyleColor();  // Revert to previous text color

                    std::lock_guard<std::mutex> lock(common->mutex); // initialization

                    for (int i = common->jobList.size() - 1; i >= 0; --i) {// Loop Through the Job List in Reverse
                        auto& job = common->jobList[i];

                        bool display_job = false;

                        if (show_favorites_only) {  // fav jobs button 
                            display_job = job.is_favorite;
                        }
                        else {// no fav jobs selected
                            display_job = common->search_query.empty() || job.title.find(common->search_query) != std::string::npos;
                        }

                        if (display_job) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);

                            // Push a bold style for job titles
                            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
                            ImGui::Text(job.title.c_str());
                            ImGui::PopStyleVar();

                            ImGui::TableSetColumnIndex(1);
                            // Make the "Show" button orange
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
                            if (ImGui::Button(("Show##" + std::to_string(i)).c_str())) {
                                selected_job_index = i;  // Set the selected job index
                            }
                            ImGui::PopStyleColor(); // Revert to previous button color

                            ImGui::TableSetColumnIndex(2);
                            std::string favorite_button_label = job.is_favorite ? "Unfavorite##" + std::to_string(i) : "Favorite##" + std::to_string(i);
                            ImGui::PushStyleColor(ImGuiCol_Button, job.is_favorite ? ImVec4(0.8f, 0.4f, 0.4f, 1.0f) : ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
                            if (ImGui::Button(favorite_button_label.c_str())) {
                                job.is_favorite = !job.is_favorite;
                            }
                            ImGui::PopStyleColor();

                            ImGui::TableSetColumnIndex(3);
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                            std::string remove_button_label = "Remove##" + std::to_string(i);
                            if (ImGui::Button(remove_button_label.c_str())) {
                                common->jobList.erase(common->jobList.begin() + i);
                            }
                            ImGui::PopStyleColor();
                        }
                    }
                    ImGui::EndTable();

                    // Revert header colors back to default
                    ImGui::PopStyleColor(2);
                }
            }

            ImGui::End();
        }
        else {
            // Retrieve the selected job
            auto& job = common->jobList[selected_job_index];

            // Set the job details window size and position
            ImVec2 window_size = ImVec2(700, 450); // Increase the width and height
            ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

            // Center the window on the screen
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 window_pos = ImVec2((io.DisplaySize.x - window_size.x) / 2, (io.DisplaySize.y - window_size.y) / 2);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);

            // Professional-looking title bar
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.25f, 0.5f, 1.0f));  // Dark blue for active title
            ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.2f, 0.4f, 1.0f));  // Slightly lighter blue for title background
            ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.0f, 0.2f, 0.4f, 1.0f));  // Same color for collapsed state

            ImGui::Begin("Job Details");

            // Remove the title bar colors after using
            ImGui::PopStyleColor(3);

            // Use bold text for job titles
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Set text color to black
           
            ImGui::Text("Job Title: %s", job.title.c_str());
            ImGui::Text("Job Type: %s", job.job_type.c_str());
            ImGui::Text("Location: %s", job.location.c_str());
            ImGui::Text("Experience: %s", job.experience.c_str());

            ImGui::Separator();
            ImGui::TextWrapped("Description: %s", job.job_description.c_str());

            ImGui::PopStyleColor();  // Revert the text color to the previous color
            ImGui::PopStyleVar();  // Revert bold style

            ImGui::Spacing(); // Add some spacing between the text blocks

            // Style the "Back" button to look more professional
            ImGui::SetCursorPosY(window_size.y - 50);  // Position the "Back" button near the bottom
            ImGui::SetCursorPosX((window_size.x - 100) / 2);  // Center the "Back" button horizontally
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 1.0f, 1.0f));  // Blue button background
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // White text color
            if (ImGui::Button("Back", ImVec2(100, 40))) {
                selected_job_index = -1;  // Go back to the main screen
            }
            ImGui::PopStyleColor(2);  // Revert button and text color

            ImGui::End();
        }

        ImGui::PopStyleColor(5); // Revert to original window colors, frame colors, text colors
    }
}

void DrawThread::operator()(CommonObjects& common) {
    GuiMain(DrawAppWindow, &common); // Call the GUI main loop with the DrawAppWindow function and common objects.
    common.exit_flag = true; // Set the exit flag to true when the drawing thread ends.
}
