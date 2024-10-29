import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;

import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.UIManager;
import javax.swing.Icon;
import javax.swing.JCheckBox;
import java.util.Objects;

import java.io.IOException;
import java.lang.InterruptedException;


public class GUIInterface {
	
	static boolean decompile_missions = false;
	static String base_script_path;
	static String mission_path;
	static String output_path;
	
	public static void main(String[] args) {
		JFrame window = new JFrame("T.M. Decompiler User Interface");
		
		JPanel first_panel = new JPanel();
		JPanel second_panel = new JPanel();
		JPanel third_panel = new JPanel();
		
		window.setLayout(new GridLayout());		//  FlowLayout
		
		JLabel l_load = new JLabel("   SCR:");
		JLabel l_out = new JLabel("Output:");
		
		JTextField f_load = new JTextField(27);
		JTextField f_out = new JTextField(27);
		
		JCheckBox cb_dec_missions = new JCheckBox("Decompile missions");
		
		cb_dec_missions.setEnabled(false);
		
		Icon folder_icon = UIManager.getIcon("FileView.directoryIcon");
		
		JButton b_load = new JButton(folder_icon);			//  Load Main Script
		JButton b_out = new JButton(folder_icon);		    //  Output
		JButton b_decompile = new JButton("Decompile");
		
		//  Button to load .scr file
		b_load.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				final JFileChooser fc = new JFileChooser(System.getProperty("user.dir"));
				FileNameExtensionFilter filter = new FileNameExtensionFilter("SCR script files", "scr");
				fc.setFileFilter(filter);
				fc.showOpenDialog(null);
				File base_script_file = fc.getSelectedFile();
				
				//  If a file has been selected
				if ( base_script_file != null ) {
					String absolute_path = base_script_file.getAbsolutePath();
					String mission_path = absolute_path.replaceFirst("[.][^.]+$", "");
					String output_path = base_script_file.getParent();
				
					mission_path = mission_path.concat("/");
					output_path = output_path.concat("/");
					
					File tempFile = new File(mission_path);
					
					//  Formating path folders
					mission_path = mission_path.replace("\\","/");
					absolute_path = absolute_path.replace("\\","/");
					output_path = output_path.replace("\\","/");
					
					
					if ( tempFile.exists() ) {
						
						GUIInterface.mission_path = mission_path;
						
						if ( JOptionPane.showConfirmDialog(null, "Mission script folder was found. Do you want decompile them as well?", "Mission folder found",JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION ) {
							GUIInterface.decompile_missions = true;
							
							//  If the box isn't selected, do select
							if ( !cb_dec_missions.isSelected() ) {
								cb_dec_missions.setSelected(true);			// cb_dec_missions.doClick();
							}
							
						} else {
							GUIInterface.decompile_missions = false;
						}
						
						cb_dec_missions.setEnabled(true);
						
					} else {
						//  No mission folder
						cb_dec_missions.setSelected(false);
						cb_dec_missions.setEnabled(false);
						GUIInterface.decompile_missions = false;
					}
					
					
					GUIInterface.base_script_path = absolute_path;
					f_load.setText(absolute_path);
					
					//  If output directory hasn't been set yet
					if ( ( f_out.getText() == null ) || ( f_out.getText().equals("") ) ) {
						GUIInterface.output_path = output_path;
						f_out.setText(output_path);
					}
					
					
					
				}
			}
		});
		
		
		//  Button to select output path
		b_out.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				final JFileChooser fc = new JFileChooser(System.getProperty("user.dir"));
				fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				fc.showOpenDialog(null);
				File base_script_file = fc.getSelectedFile();
				
				//  If a file has been selected
				if ( base_script_file != null ) {
					String output_path = base_script_file.getAbsolutePath();
					output_path = output_path.concat("/");
					output_path = output_path.replace("\\","/");		//  Formating path folder
					
					GUIInterface.output_path = output_path;
					
					f_out.setText(output_path);
					
				}
			}
		});
		
		//  CheckBox to decompile missions
		cb_dec_missions.addItemListener( new java.awt.event.ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent event) {
				if (cb_dec_missions.isSelected()) {
					GUIInterface.decompile_missions = true;			// e.getStateChange()==1
				} else {
					GUIInterface.decompile_missions = false;
				}
			}
		} );
		
		
		//  Button to decompile
		b_decompile.addActionListener(new java.awt.event.ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				
				boolean error = false;
				
				//  Checking for unfullfilled fields
				if ( ( f_load.getText() == null ) || ( f_load.getText().equals("") ) ) {
					JOptionPane.showMessageDialog(null,"Error: Select a valid .scr file.", "Ok", JOptionPane.ERROR_MESSAGE);
					error = true;
				}
				
				File tempFile = new File(f_load.getText());
				
				if ( !tempFile.exists() ) {
					JOptionPane.showMessageDialog(null,"Error: Select a valid .scr file.", "Ok", JOptionPane.ERROR_MESSAGE);
					error = true;
				}
				
				if ( ( f_out.getText() == null ) || ( f_out.getText().equals("") ) ) {
					JOptionPane.showMessageDialog(null,"Error: Select a valid output folder.", "Ok", JOptionPane.ERROR_MESSAGE);
					error = true;
				}
				
				tempFile = new File(f_out.getText());
				
				if ( !tempFile.exists() ) {
					JOptionPane.showMessageDialog(null,"Error: Select a valid output folder.", "Ok", JOptionPane.ERROR_MESSAGE);
					error = true;
				}
				
				//  If everything is ok
				if (!error) {
					
					ProcessBuilder pb;
					
					String decompiler_folder = System.getProperty("user.dir");
					decompiler_folder = decompiler_folder.concat("\\");
					decompiler_folder = decompiler_folder.concat("scr_decompiler.exe");
					
					if ( GUIInterface.decompile_missions == false ) {
						pb = new ProcessBuilder("cmd", "/c", "scr_decompiler.exe", f_out.getText(), f_load.getText());
					} else {
						pb = new ProcessBuilder("cmd", "/c", "scr_decompiler.exe", f_out.getText(), f_load.getText(), GUIInterface.mission_path);
					}
					
					try {
						pb.start();		//  Starts SCR decompiler
						JOptionPane.showMessageDialog(null,"SCR files decompiled successfully!", "Ok", JOptionPane.INFORMATION_MESSAGE);
					} catch (IOException ex) {
						JOptionPane.showMessageDialog(null,"Error.", "Ok", JOptionPane.ERROR_MESSAGE);
					}
				}
				
			}
		});
		
		first_panel.add(l_load);
		first_panel.add(f_load);
		first_panel.add(b_load);
		
		
		first_panel.add(l_out);
		first_panel.add(f_out);
		first_panel.add(b_out);
		
		first_panel.add(cb_dec_missions);
		first_panel.add(b_decompile);
		
		window.add(first_panel);
		window.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		window.setSize(450,175);
		window.setVisible(true);
		
	}

}

